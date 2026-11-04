#include "fable_variable_size_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct MemoryInfo{void* BaseAddress;void* AllocationBase;unsigned long AllocationProtect,RegionSize,State,Protect,Type;};
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) unsigned long __stdcall VirtualQuery(const void*,MemoryInfo*,unsigned long);
typedef CMemoryAllocatorVariableSize::CMemoryArea Area;
typedef CMemoryAllocatorVariableSize::CMemoryAreaChunk Chunk;
static unsigned long operations,queries,releases,scenarios;
static void require(bool value,const char* message){if(!value){printf("OS_LIFETIME_FAIL scenario=%lu operations=%lu %s\n",scenarios,operations,message);exit(5);}}
static unsigned long state(void* pointer){MemoryInfo info;require(VirtualQuery(pointer,&info,sizeof(info))==sizeof(info),"VirtualQuery");return info.State;}
static Area* freeNodes[256];static unsigned long freeCount,visited;
static long blackDepth(Area* node,Area* parent,Area* lower,Area* upper)
{
    if(!node)return 1;
    bool member=false;for(unsigned long i=0;i<freeCount;++i)if(freeNodes[i]==node){member=true;freeNodes[i]=0;break;}
    require(member,"tree membership/cycle");++visited;
    require(node->PParentArea==parent,"tree parent");
    require(!lower||lower->DataSize<node->DataSize||(lower->DataSize==node->DataSize&&lower<node),"tree lower key");
    require(!upper||node->DataSize<upper->DataSize||(node->DataSize==upper->DataSize&&node<upper),"tree upper key");
    require(node->Colour<=1,"tree colour");
    if(!node->Colour)require((!node->PLeftChild||node->PLeftChild->Colour)&&(!node->PRightChild||node->PRightChild->Colour),"red children");
    long left=blackDepth(node->PLeftChild,node,lower,node),right=blackDepth(node->PRightChild,node,node,upper);
    require(left==right,"black depth");return left+(node->Colour==1);
}
static void validate(CMemoryAllocatorVariableSize* owner,unsigned long alignment)
{
    unsigned long areaHeader=(32+alignment-1)/alignment*alignment,chunkHeader=(52+alignment-1)/alignment*alignment;
    unsigned long chunkCount=0,allocatedCount=0,allocatedSize=0,controlled=0,largest=0;freeCount=0;
    Chunk* previous=0;
    for(Chunk* chunk=owner->PFirstChunk;chunk;chunk=chunk->PNextChunk){
        require(++chunkCount<64,"chunk cycle");require(chunk->PPreviousChunk==previous&&chunk->PParentAllocator==owner,"chunk links");previous=chunk;
        require(state(chunk)==0x1000,"chunk committed");
        unsigned long starts[256],ends[256],extents=0;
        unsigned long begin=reinterpret_cast<unsigned long>(chunk)+chunkHeader,end=begin+chunk->TotalDataSize+areaHeader;
        unsigned long count=0,bytes=0,min=0,max=0;Area* prev=0;
        for(Area* area=chunk->PFirstFreeArea;area;area=area->PNextArea){
            require(++count<256&&freeCount<256,"free cycle");freeNodes[freeCount++]=area;
            require(extents<256,"extent capacity");starts[extents]=reinterpret_cast<unsigned long>(area);ends[extents]=starts[extents]+areaHeader+area->DataSize;++extents;
            require(area->PPreviousArea==prev&&area->PParentChunk==chunk&&area->DataSize>=0,"free links/size");
            require(!prev||prev->DataSize<=area->DataSize,"free size order");
            if(!prev)min=area->DataSize;max=area->DataSize;bytes+=area->DataSize;prev=area;
        }
        require(chunk->NoFreeAreas==count&&chunk->FreeDataSize==bytes&&chunk->PLastFreeArea==prev,"free metadata");
        require(chunk->SmallestFreeAreaSize==min&&chunk->LargestFreeAreaSize==max,"free extrema");if(max>largest)largest=max;
        unsigned long totalNodes=count,totalBytes=bytes;count=0;prev=0;
        for(Area* area=chunk->PFirstAllocatedArea;area;area=area->PNextArea){
            require(++count<256,"allocated cycle");require(area->PPreviousArea==prev&&area->PParentChunk==chunk&&area->DataSize>=0,"allocated links");
            require(extents<256,"extent capacity");starts[extents]=reinterpret_cast<unsigned long>(area);ends[extents]=starts[extents]+areaHeader+area->DataSize;++extents;
            totalBytes+=area->DataSize;allocatedSize+=area->DataSize;prev=area;
        }
        require(chunk->NoAllocatedAreas==count,"allocated metadata");allocatedCount+=count;totalNodes+=count;
        require(totalBytes+totalNodes*areaHeader==chunk->TotalDataSize+areaHeader,"data/header conservation");
        for(unsigned long a=0;a<extents;++a){
            require(starts[a]>=begin&&ends[a]>=starts[a]&&ends[a]<=end,"extent bounds");
            for(unsigned long b=0;b<a;++b)require(ends[a]<=starts[b]||ends[b]<=starts[a],"area extent overlap");
        }
        controlled+=chunk->TotalDataSize+areaHeader+chunkHeader;
    }
    if(freeCount){require(owner->PTreeRoot&&owner->PTreeRoot->Colour==1,"black root");visited=0;blackDepth(owner->PTreeRoot,0,0,0);require(visited==freeCount,"tree count");}
    else require(owner->PTreeRoot==0,"empty root");
    CMemoryAllocatorBase* api=owner;
    require(api->GetNoFreeAreas()==freeCount&&api->GetNoAllocatedAreas()==0,"retail count behavior");
    require(api->GetLargestFreeAreaSize()==largest,"largest query");
    require(api->GetHeaderOverhead()==chunkCount*chunkHeader+allocatedCount*areaHeader,"header overhead");
    require(api->GetTotalMemoryAllocated()==allocatedSize+chunkCount*chunkHeader+allocatedCount*areaHeader,"allocated total");
    require(api->GetTotalMemoryControlledByAllocator()==controlled,"controlled total");
    void* output[256];memset(output,0xcc,sizeof(output));unsigned long enumerated=api->GetAllAllocatedAreas(output,0);
    require(enumerated==allocatedCount,"enumeration count");unsigned long index=0;
    for(Chunk* chunk=owner->PFirstChunk;chunk;chunk=chunk->PNextChunk)
        for(Area* area=chunk->PFirstAllocatedArea;area;area=area->PNextArea){
            void* data=reinterpret_cast<unsigned char*>(area)+areaHeader;
            require(output[index]==data&&api->GetAllocatedArea(index)==data,"enumeration pointer");
            require(api->GetAllocationSize(data)==area->DataSize&&api->GetAllocationSizeIncludingHeader(data)==area->DataSize+areaHeader,"size queries");++index;
        }
    require(api->GetAllocatedArea(-1)==0&&api->GetAllocatedArea(index)==0,"index miss");++queries;
}
struct Allocation{void* pointer;unsigned long size,pattern;bool live;};
static void payloads(Allocation* items,unsigned long count)
{
    for(unsigned long i=0;i<count;++i)if(items[i].live){
        unsigned char* bytes=static_cast<unsigned char*>(items[i].pointer);
        for(unsigned long j=0;j<items[i].size;++j)require(bytes[j]==items[i].pattern,"payload preserved");
        for(unsigned long k=0;k<i;++k)if(items[k].live&&items[k].size&&items[i].size)
            require(bytes+items[i].size<=items[k].pointer||static_cast<unsigned char*>(items[k].pointer)+items[k].size<=bytes,"payload overlap");
    }
}
int main()
{
    unsigned long requests[15]={0,1,31,32,33,63,64,127,128,255,512,1024,4096,8192,16384};
    // Retail base constructor supports only4/8/16; other values use invalid shift9999999.
    for(unsigned long alignment=4;alignment<=16;alignment*=2)
     for(unsigned long initial=0;initial<3;++initial)
      for(unsigned long order=0;order<3;++order)
       for(unsigned long freeAll=0;freeAll<2;++freeAll){
        ++scenarios;void* supplied=initial==2?VirtualAlloc(0,4096,0x3000,4):0;require(initial!=2||supplied!=0,"caller memory");
        CMemoryAllocatorVariableSize* owner=new CMemoryAllocatorVariableSize(0,4096,supplied,initial?4096:0,alignment,0,0);require(owner!=0,"owner allocation");
        void* ownerAddress=owner;void* chunks[64];unsigned long chunkCount=0;Allocation items[15];memset(items,0,sizeof(items));validate(owner,alignment);
        for(unsigned long i=0;i<15;++i){
            void* data=static_cast<CMemoryAllocatorBase*>(owner)->Allocate(requests[i]);require(data!=0&&reinterpret_cast<unsigned long>(data)%alignment==0,"allocation/alignment");
            items[i].pointer=data;items[i].size=requests[i];items[i].pattern=(i*13+37)&255;items[i].live=true;memset(data,items[i].pattern,requests[i]);++operations;
            payloads(items,i+1);validate(owner,alignment);
            for(Chunk* c=owner->PFirstChunk;c;c=c->PNextChunk){bool seen=false;for(unsigned long j=0;j<chunkCount;++j)if(chunks[j]==c)seen=true;if(!seen){require(chunkCount<64,"chunk history");chunks[chunkCount++]=c;}}
        }
        if(freeAll)for(unsigned long n=0;n<15;++n){
            unsigned long index=order==0?n:order==1?14-n:(n*7)%15;
            payloads(items,15);Area* area=reinterpret_cast<Area*>(static_cast<unsigned char*>(items[index].pointer)-(32+alignment-1)/alignment*alignment);
            Chunk* chunk=area->PParentChunk;bool shouldRelease=chunk->NoAllocatedAreas==1&&!chunk->Permanent&&!chunk->DemandPaged;
            static_cast<CMemoryAllocatorBase*>(owner)->Deallocate(items[index].pointer);items[index].live=false;++operations;
            if(shouldRelease){require(state(chunk)==0x10000,"last allocation releases chunk");++releases;}
            payloads(items,15);validate(owner,alignment);
        }
        else payloads(items,15);
        delete owner;require(state(ownerAddress)==0x10000,"owner released");++releases;
        for(unsigned long j=0;j<chunkCount;++j)require(state(chunks[j])==(chunks[j]==supplied?0x1000:0x10000),"destructor chunk ownership");
        if(supplied){require(VirtualFree(supplied,0,0x8000)!=0,"caller cleanup");require(state(supplied)==0x10000,"caller memory released");}
    }
    printf("VARIABLE_OS_LIFETIME_OK scenarios=%lu operations=%lu validations=%lu observed_releases=%lu\n",scenarios,operations,queries,releases);return 0;
}
