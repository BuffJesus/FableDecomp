#include "fable_memory_allocator.h"
#include "candidates/fixed_size_allocator_accessors_compiled.h"
#include "fable_variable_size_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct MemoryInfo{void* base;void* allocation;unsigned long allocationProtection,regionSize,state,protection,type;};
extern "C" __declspec(dllimport) unsigned long __stdcall VirtualQuery(const void*,MemoryInfo*,unsigned long);
extern "C" __declspec(dllimport) void* __stdcall GetProcessHeap();
extern "C" __declspec(dllimport) int __stdcall HeapValidate(void*,unsigned long,const void*);
extern "C" __declspec(dllimport) int __stdcall HeapFree(void*,unsigned long,void*);
class Access:public CMemoryAllocator{
public:
    static void Start(){Create();}
    static CAllocatorInfo* Table(){return AllocatorInfo;}
    static unsigned char* AllocateMain(CMemoryAllocator* owner,long size,unsigned long category,bool debug){EnterSynchronisation();unsigned char* p=static_cast<unsigned char*>(reinterpret_cast<Access*>(owner)->InternalAllocate(size,category,debug));LeaveSynchronisation();return p;}
    static void FreeMain(CMemoryAllocator* owner,void* p){EnterSynchronisation();reinterpret_cast<Access*>(owner)->InternalDeallocate(p);LeaveSynchronisation();}
};
struct Live{unsigned char* data;unsigned char* aligned;unsigned long payload,alignment;unsigned long size,row,tracking;unsigned char tag;};
static Live live[128];static unsigned long liveCount,tracked[256],operations,checks,scenario,retained;
static void require(bool yes){if(!yes){printf("MAIN_ALIGNED_OS_FAIL scenario=%lu operation=%lu\n",scenario,operations);exit(2);}}
static unsigned long word(CMemoryAllocator* owner,unsigned long offset){return *reinterpret_cast<unsigned long*>(reinterpret_cast<unsigned char*>(owner)+offset);}
static unsigned long state(void* address){MemoryInfo info;require(VirtualQuery(address,&info,sizeof(info))==sizeof(info));return info.state;}
static void verify(CMemoryAllocator* main)
{
    ++checks;require(word(main,0x40c)==liveCount);
    for(unsigned long id=0;id<256;++id)require(word(main,id*4)==tracked[id]);
    require(*reinterpret_cast<long*>(reinterpret_cast<unsigned char*>(&g_CMemoryAllocatorCriticalSection_013CAA70)+8)==0);
    for(unsigned long i=0;i<liveCount;++i){
        Live& item=live[i];require(item.data[-5]==item.row&&*reinterpret_cast<unsigned long*>(item.data-4)==0xffffffff);
        unsigned long capacity=main->GetAllocationSize(item.data);require(capacity>=item.size+20);
        unsigned long* metadata=reinterpret_cast<unsigned long*>(item.data+capacity-20);require(metadata[0]==0xffffffff&&metadata[1]==item.tracking&&metadata[2]==item.size);
        require(((unsigned long)item.aligned&(item.alignment-1))==0);require(*(unsigned char**)(item.aligned-4)==item.data);require(item.aligned>=item.data+4&&item.aligned+item.payload<=item.data+item.size);for(unsigned long j=0;j<item.payload;++j)require(item.aligned[j]==item.tag);
        for(unsigned long j=0;j<i;++j){unsigned long a=reinterpret_cast<unsigned long>(item.data),b=reinterpret_cast<unsigned long>(live[j].data);require(a+item.size<=b||b+live[j].size<=a);}
    }
    unsigned long aggregate=0,count=0;
    for(unsigned long row=0;row<30;++row){
        CMemoryAllocatorBase* owner=Access::Table()[row].PAllocator;unsigned long expected=0;for(unsigned long i=0;i<liveCount;++i)if(live[i].row==row)++expected;
        void* pointers[128];memset(pointers,0xa5,sizeof(pointers));require(owner->GetAllAllocatedAreas(pointers,0)==expected);
        for(unsigned long j=0;j<expected;++j){bool found=false;for(unsigned long i=0;i<liveCount;++i)if(live[i].row==row&&live[i].data-4==pointers[j]){found=true;break;}require(found&&owner->GetAllocatedArea(j)==pointers[j]);for(unsigned long k=0;k<j;++k)require(pointers[j]!=pointers[k]);}
        require(pointers[expected]==reinterpret_cast<void*>(0xa5a5a5a5)&&owner->GetAllocatedArea(expected)==0);
        require(owner->GetNoAllocatedAreas()==(row<28?expected:0));owner->Validate();aggregate+=owner->GetTotalMemoryAllocated();count+=owner->GetNoAllocatedAreas();
    }
    require(main->GetTotalMemoryAllocated()==aggregate&&main->GetNoAllocatedAreas()==count&&HeapValidate(GetProcessHeap(),0,0)!=0);
}
static void releaseAll(CMemoryAllocator* main,unsigned long order)
{
    while(liveCount){unsigned long chosen=order==0?0:order==1?liveCount-1:liveCount/2;Live item=live[chosen];CMemoryAllocator::DeallocateAligned(item.aligned);tracked[item.tracking]-=item.size;for(unsigned long j=chosen+1;j<liveCount;++j)live[j-1]=live[j];--liveCount;++operations;verify(main);}
}
int main()
{
    const unsigned long requests[]={0,1,4,12,13,44,45,235,236,237,255,256,257,4096,65536,131072};
    CMemoryAllocator::CAllocatorInfo initial[30];memcpy(initial,Access::Table(),sizeof(initial));
    for(unsigned long order=0;order<3;++order)for(unsigned long defer=0;defer<1;++defer){
        memcpy(Access::Table(),initial,sizeof(initial));g_FableAllocatorShutdownRequested_013CAA94=false;Access::Start();CMemoryAllocator* main=g_FableMemoryAllocator_013CAA90;require(main!=0);
        liveCount=0;memset(tracked,0,sizeof(tracked));verify(main);
        for(unsigned long cycle=0;cycle<2;++cycle){
            for(unsigned long categoryIndex=0;categoryIndex<2;++categoryIndex)for(unsigned long i=0;i<16;++i){
                Live& item=live[liveCount];item.payload=requests[i];item.alignment=1UL<<((i+categoryIndex*4+cycle)%13);item.size=item.payload+item.alignment+3;item.tracking=(i*17+cycle*13)%256;
                reinterpret_cast<unsigned char*>(main)[0x414]=static_cast<unsigned char>(item.tracking);
                item.row=28;for(unsigned long row=0;row<28;++row)if(item.size+20<=static_cast<unsigned long>(initial[row].MaxAllocSize)){item.row=row;break;}
                item.aligned=(unsigned char*)CMemoryAllocator::AllocateAligned(item.payload,item.alignment);require(item.aligned!=0);item.data=*(unsigned char**)(item.aligned-4);require(item.data!=0);item.tag=static_cast<unsigned char>(i+cycle+17);memset(item.aligned,item.tag,item.payload);tracked[item.tracking]+=item.size;++liveCount;++operations;verify(main);
            }
            if(defer&&cycle==1){FableMemoryAllocatorShutdown_009f2d20();require(g_FableMemoryAllocator_013CAA90==main&&g_FableAllocatorShutdownRequested_013CAA94);verify(main);}
            releaseAll(main,order);
        }
        void* objects[30];void* chunks[128];unsigned long kinds[128],chunkCount=0;
        for(unsigned long row=0;row<30;++row){
            objects[row]=Access::Table()[row].PAllocator;
            if(row<28){CMemoryAllocatorFixedSize* owner=static_cast<CMemoryAllocatorFixedSize*>(Access::Table()[row].PAllocator);for(CMemoryAllocatorFixedSize::CMemoryAreaChunk* c=owner->PFirstChunk;c;c=c->PNextChunk){require(chunkCount<128);chunks[chunkCount]=c->ThisChunkMem;kinds[chunkCount++]=0;}}
            else{CMemoryAllocatorVariableSize* owner=static_cast<CMemoryAllocatorVariableSize*>(Access::Table()[row].PAllocator);for(CMemoryAllocatorVariableSize::CMemoryAreaChunk* c=owner->PFirstChunk;c;c=c->PNextChunk){require(chunkCount<128);chunks[chunkCount]=c;kinds[chunkCount++]=1;}}
        }
        FableMemoryAllocatorShutdown_009f2d20();require(g_FableMemoryAllocator_013CAA90==0&&state(main)==0x10000);
        for(unsigned long row=0;row<30;++row)require(state(objects[row])==0x10000);
        for(unsigned long i=0;i<chunkCount;++i){++retained;if(kinds[i]){require(state(chunks[i])==0x1000&&VirtualFree(chunks[i],0,0x8000)!=0);}else{require(HeapValidate(GetProcessHeap(),0,chunks[i])!=0&&HeapFree(GetProcessHeap(),1,chunks[i])!=0);}}
        require(HeapValidate(GetProcessHeap(),0,0)!=0);++scenario;
    }
    printf("MAIN_ALIGNED_OS_OK scenarios=%lu operations=%lu checks=%lu retained_chunks=%lu\n",scenario,operations,checks,retained);return 0;
}
