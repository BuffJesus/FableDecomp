#include "candidates/fixed_size_allocator_accessors_compiled.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall GetProcessHeap();
extern "C" __declspec(dllimport) unsigned long __stdcall HeapSize(void*,unsigned long,const void*);
extern "C" __declspec(dllimport) int __stdcall HeapValidate(void*,unsigned long,const void*);
extern "C" __declspec(dllimport) int __stdcall HeapFree(void*,unsigned long,void*);
struct Live{unsigned char* data;unsigned char tag;};
static Live live[256];static unsigned long liveCount,operations,validations,scenarios,retained;
static void require(bool yes){if(!yes){printf("fixed OS invariant failed scenario=%lu operation=%lu\n",scenarios,operations);exit(2);}}
static unsigned long verify(CMemoryAllocatorBase* base,CMemoryAllocatorFixedSize* owner)
{
    ++validations;unsigned long chunks=0,used=0,free=0;void* enumeration[256];unsigned long enumCount=0;
    CMemoryAllocatorFixedSize::CMemoryAreaChunk* previous=0;
    bool currentSeen=owner->PCurrentAllocatorChunk==0;
    for(CMemoryAllocatorFixedSize::CMemoryAreaChunk* chunk=owner->PFirstChunk;chunk;chunk=chunk->PNextChunk){
        require(++chunks<32&&chunk->PPreviousChunk==previous);previous=chunk;
        if(owner->PCurrentAllocatorChunk==chunk)currentSeen=true;
        require(HeapValidate(GetProcessHeap(),0,chunk->ThisChunkMem)!=0);
        unsigned long extent=HeapSize(GetProcessHeap(),0,chunk->ThisChunkMem),offset=reinterpret_cast<unsigned long>(chunk)-reinterpret_cast<unsigned long>(chunk->ThisChunkMem);
        require(offset<16&&offset+owner->ChunkSize<=extent);bool seen[256];memset(seen,0,sizeof(seen));unsigned long counts[2]={0,0};
        for(unsigned long list=0;list<2;++list){
            CMemoryAllocatorFixedSize::CMemoryArea* node=list?chunk->PFirstFreeArea:chunk->PFirstAllocatedArea;unsigned long previousID=255;
            while(node){
                unsigned long id=node->AreaID;require(id<owner->NoAreasPerChunk&&!seen[id]&&++counts[list]<=owner->NoAreasPerChunk);seen[id]=true;
                require(reinterpret_cast<unsigned char*>(node)==reinterpret_cast<unsigned char*>(chunk)+owner->ChunkHeaderSize+id*owner->StatAreaSize&&node->AllocatorID==0);
                if(!list||counts[list]>1)require(node->PreviousAreaID==previousID);
                if(!list){
                    enumeration[enumCount++]=reinterpret_cast<unsigned char*>(node)+4;bool found=false;
                    for(unsigned long i=0;i<liveCount;++i)if(live[i].data==enumeration[enumCount-1]){found=true;break;}require(found);
                }
                previousID=id;
                node=node->NextAreaID==255?0:reinterpret_cast<CMemoryAllocatorFixedSize::CMemoryArea*>(reinterpret_cast<unsigned char*>(chunk)+owner->ChunkHeaderSize+node->NextAreaID*owner->StatAreaSize);
            }
        }
        require(counts[0]+counts[1]==owner->NoAreasPerChunk&&counts[0]==static_cast<unsigned long>(chunk->NoAllocatedAreas));used+=counts[0];free+=counts[1];
    }
    require(previous==owner->PLastChunk&&currentSeen&&used==liveCount);
    for(unsigned long i=0;i<liveCount;++i){
        require(reinterpret_cast<unsigned long>(live[i].data)%*reinterpret_cast<unsigned long*>(reinterpret_cast<unsigned char*>(owner)+0xc14)==0);
        for(unsigned long j=0;j<owner->AreaSize;++j)require(live[i].data[j]==live[i].tag);
        for(unsigned long j=0;j<i;++j)require(live[i].data+owner->AreaSize<=live[j].data||live[j].data+owner->AreaSize<=live[i].data);
        require(base->GetAllocationSize(live[i].data)==owner->AreaSize&&base->GetAllocationSizeIncludingHeader(live[i].data)==owner->StatAreaSize);
    }
    require(base->GetNoAllocatedAreas()==used&&base->GetNoFreeAreas()==free);
    require(base->GetTotalMemoryControlledByAllocator()==chunks*owner->ChunkSize+owner->ExtraMemUsedByOS);
    require(base->GetTotalMemoryAllocated()==used*owner->StatAreaSize+chunks*owner->ChunkHeaderSize);
    require(base->GetHeaderOverhead()==used*4+chunks*owner->ChunkHeaderSize);
    require(base->GetLargestFreeAreaSize()==(free?owner->AreaSize:0));
    void* actual[256];memset(actual,0xa5,sizeof(actual));require(base->GetAllAllocatedAreas(actual,0)==used);
    for(unsigned long i=0;i<used;++i)require(actual[i]==enumeration[i]&&base->GetAllocatedArea(i)==enumeration[i]);
    require(actual[used]==reinterpret_cast<void*>(0xa5a5a5a5)&&base->GetAllocatedArea(-1)==0&&base->GetAllocatedArea(used)==0);
    base->Validate();base->ConsolidateFreeAreas();require(HeapValidate(GetProcessHeap(),0,0)!=0);return chunks;
}
int main()
{
    const unsigned long alignments[]={4,8,16},sizes[]={16,60},minimums[]={128,256};
    unsigned char storage[sizeof(CMemoryAllocatorFixedSize)],saved[sizeof(CMemoryAllocatorFixedSize)];
    for(unsigned long a=0;a<3;++a)for(unsigned long s=0;s<2;++s)for(unsigned long m=0;m<2;++m)for(unsigned long order=0;order<3;++order)for(unsigned long leaveLive=0;leaveLive<2;++leaveLive){
        memset(storage,0xa5,sizeof(storage));CMemoryAllocatorFixedSize* owner=::new(storage)CMemoryAllocatorFixedSize(0,minimums[m],sizes[s],alignments[a],order,leaveLive);CMemoryAllocatorBase* base=owner;
        liveCount=0;unsigned long chunks=verify(base,owner);require(chunks==0&&owner->NoEmptyChunks==0);
        for(unsigned long cycle=0;cycle<2;++cycle){
            unsigned long number=owner->NoAreasPerChunk*3+1;
            for(unsigned long i=0;i<number;++i){
                long empty=owner->NoEmptyChunks;unsigned long before=chunks;
                live[liveCount].data=static_cast<unsigned char*>(base->Allocate(i%3==0?0:i%3==1?1:0xffffffff));require(live[liveCount].data!=0);
                CMemoryAllocatorFixedSize::CMemoryArea* area=reinterpret_cast<CMemoryAllocatorFixedSize::CMemoryArea*>(live[liveCount].data-4);
                CMemoryAllocatorFixedSize::CMemoryAreaChunk* chunk=reinterpret_cast<CMemoryAllocatorFixedSize::CMemoryAreaChunk*>(reinterpret_cast<unsigned char*>(area)-area->AreaID*owner->StatAreaSize-owner->ChunkHeaderSize);
                bool first=chunk->NoAllocatedAreas==1;live[liveCount].tag=static_cast<unsigned char>(i+17+cycle);memset(live[liveCount].data,live[liveCount].tag,owner->AreaSize);++liveCount;++operations;
                chunks=verify(base,owner);require(chunks==before||chunks==before+1);require(owner->NoEmptyChunks==empty+static_cast<long>(chunks-before)-(first?1:0));
            }
            if(leaveLive&&cycle==1)break;
            while(liveCount){
                unsigned long chosen=order==0?0:order==1?liveCount-1:liveCount/2;unsigned char* data=live[chosen].data;
                CMemoryAllocatorFixedSize::CMemoryArea* area=reinterpret_cast<CMemoryAllocatorFixedSize::CMemoryArea*>(data-4);
                CMemoryAllocatorFixedSize::CMemoryAreaChunk* chunk=reinterpret_cast<CMemoryAllocatorFixedSize::CMemoryAreaChunk*>(reinterpret_cast<unsigned char*>(area)-area->AreaID*owner->StatAreaSize-owner->ChunkHeaderSize);
                bool last=chunk->NoAllocatedAreas==1;long empty=owner->NoEmptyChunks;unsigned long before=chunks;
                base->Deallocate(data);for(unsigned long j=chosen+1;j<liveCount;++j)live[j-1]=live[j];--liveCount;++operations;
                chunks=verify(base,owner);require(owner->NoEmptyChunks==empty+(last?1:0));require(chunks==before-((last&&empty+1>1)?1:0));
            }
        }
        void* raw[32];unsigned long rawCount=0;for(CMemoryAllocatorFixedSize::CMemoryAreaChunk* c=owner->PFirstChunk;c;c=c->PNextChunk)raw[rawCount++]=c->ThisChunkMem;
        memcpy(saved,storage,sizeof(saved));owner->CMemoryAllocatorFixedSize::~CMemoryAllocatorFixedSize();require(memcmp(saved,storage,sizeof(saved))==0);
        for(unsigned long i=0;i<liveCount;++i)for(unsigned long j=0;j<owner->AreaSize;++j)require(live[i].data[j]==live[i].tag);
        for(unsigned long i=0;i<rawCount;++i){require(HeapValidate(GetProcessHeap(),0,raw[i])!=0);++retained;require(HeapFree(GetProcessHeap(),1,raw[i])!=0);}
        require(HeapValidate(GetProcessHeap(),0,0)!=0);++scenarios;
    }
    printf("FIXED_OS_LIFETIME_OK scenarios=%lu operations=%lu validations=%lu destructor_retained_chunks=%lu\n",scenarios,operations,validations,retained);return 0;
}
