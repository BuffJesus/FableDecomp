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
class MainAccess:public CMemoryAllocator{public:static void Start(){Create();}static CAllocatorInfo* Table(){return AllocatorInfo;}static unsigned long Baseline(){return MemoryUseBeforeAllocator;}
#ifdef FABLE_NATIVE_MAIN_INIT
static void Initialize(CMemoryAllocator* owner,void* allocation,bool debug,long requested){reinterpret_cast<MainAccess*>(owner)->InitDebugInfo(allocation,debug,requested);}
#endif
};
struct Live{CMemoryAllocatorBase* owner;unsigned char* data;unsigned long size;unsigned char tag;unsigned long row;};
static Live live[128];static unsigned long liveCount,operations,checks,scenario,retained;
#ifdef FABLE_NATIVE_MAIN_SIZE
static unsigned long sizeQueries;
#ifdef FABLE_NATIVE_MAIN_INIT
static unsigned long metadataChecks;
#endif
#endif
static void require(bool yes){if(!yes){printf("MAIN_OS_FAIL scenario=%lu operations=%lu\n",scenario,operations);exit(2);}}
static unsigned long state(void* address){MemoryInfo info;require(VirtualQuery(address,&info,sizeof(info))==sizeof(info));return info.state;}
static unsigned long word(CMemoryAllocator* owner,unsigned long offset){return *reinterpret_cast<unsigned long*>(reinterpret_cast<unsigned char*>(owner)+offset);}
static void verify(CMemoryAllocator* main)
{
    ++checks;unsigned long aggregateSize=0,aggregateCount=0;
    for(unsigned long row=0;row<30;++row){
        CMemoryAllocatorBase* owner=MainAccess::Table()[row].PAllocator;unsigned long count=0;for(unsigned long i=0;i<liveCount;++i)if(live[i].row==row)++count;
        void* enumeration[128];memset(enumeration,0xa5,sizeof(enumeration));require(owner->GetAllAllocatedAreas(enumeration,0)==count);
        for(unsigned long j=0;j<count;++j){
            bool found=false;for(unsigned long i=0;i<liveCount;++i)if(live[i].row==row&&live[i].data==enumeration[j]){found=true;break;}require(found&&owner->GetAllocatedArea(j)==enumeration[j]);
            for(unsigned long k=0;k<j;++k)require(enumeration[k]!=enumeration[j]);
        }
        require(enumeration[count]==reinterpret_cast<void*>(0xa5a5a5a5)&&owner->GetAllocatedArea(-1)==0&&owner->GetAllocatedArea(count)==0);
        require(owner->GetNoAllocatedAreas()==(row<28?count:0));owner->Validate();owner->ConsolidateFreeAreas();
        aggregateCount+=owner->GetNoAllocatedAreas();aggregateSize+=owner->GetTotalMemoryAllocated();
    }
    require(main->GetNoAllocatedAreas()==aggregateCount&&main->GetTotalMemoryAllocated()==aggregateSize);
    for(unsigned long i=0;i<liveCount;++i){
        require(live[i].owner->GetAllocationSize(live[i].data)>=live[i].size);
        require(reinterpret_cast<unsigned long>(live[i].data)%MainAccess::Table()[live[i].row].Alignment==0);
        for(unsigned long j=0;j<live[i].size;++j)require(live[i].data[j]==live[i].tag);
        for(unsigned long j=0;j<i;++j){unsigned long a=reinterpret_cast<unsigned long>(live[i].data),b=reinterpret_cast<unsigned long>(live[j].data);require(a+live[i].size<=b||b+live[j].size<=a);}
    }
    require(HeapValidate(GetProcessHeap(),0,0)!=0);
}
int main()
{
    CMemoryAllocator::CAllocatorInfo initial[30];memcpy(initial,MainAccess::Table(),sizeof(initial));
    for(unsigned long order=0;order<3;++order)for(unsigned long leaveLive=0;leaveLive<2;++leaveLive){
        memcpy(MainAccess::Table(),initial,sizeof(initial));g_FableAllocatorShutdownRequested_013CAA94=false;require(g_FableMemoryAllocator_013CAA90==0);
        MainAccess::Start();CMemoryAllocator* main=g_FableMemoryAllocator_013CAA90;require(main!=0&&state(main)==0x1000&&word(main,0x410)==30&&word(main,0x40c)==0);
        require(MainAccess::Baseline()==g_FableAllocatorMemoryBaseline_013CAAB0&&!g_FableAllocatorShutdownRequested_013CAA94);
        require(word(main,0x424)==256&&word(main,0x424+9*4)==0);
        for(unsigned long category=0;category<15;++category){bool exists=category==0||category==9;require(*(reinterpret_cast<unsigned char*>(main)+0x415+category)==exists);require((word(main,0x460+category*4)!=0)==exists);}
        CAllocSizeToAllocator* lookup=reinterpret_cast<CAllocSizeToAllocator*>(word(main,0x460));
        for(unsigned long size=0;size<258;++size){unsigned long expected=28;for(unsigned long row=0;row<28;++row)if(size<=static_cast<unsigned long>(initial[row].MaxAllocSize)){expected=row;break;}require(lookup[size].ID==expected&&lookup[size].PAllocator==MainAccess::Table()[expected].PAllocator);}
        lookup=reinterpret_cast<CAllocSizeToAllocator*>(word(main,0x460+9*4));for(unsigned long size=0;size<2;++size)require(lookup[size].ID==29&&lookup[size].PAllocator==MainAccess::Table()[29].PAllocator);
        void* objects[30];for(unsigned long row=0;row<30;++row){objects[row]=MainAccess::Table()[row].PAllocator;require(objects[row]!=0&&state(objects[row])==0x1000);}
        liveCount=0;verify(main);
        for(unsigned long cycle=0;cycle<2;++cycle){
            for(unsigned long row=0;row<30;++row)for(unsigned long i=0;i<3;++i){
                Live& item=live[liveCount];item.row=row;item.owner=MainAccess::Table()[row].PAllocator;item.size=row<28?MainAccess::Table()[row].MaxAllocSize:(i==0?257:i==1?4097:65537);
                item.data=static_cast<unsigned char*>(item.owner->Allocate(item.size));require(item.data!=0);item.tag=static_cast<unsigned char>(row+cycle+17+i);memset(item.data,item.tag,item.size);++liveCount;++operations;verify(main);
#ifdef FABLE_NATIVE_MAIN_SIZE
                // Direct native size-class allocation has no main ID prefix yet.
                // Supply its real area-header ID, then exercise the main dispatcher.
                unsigned char savedID=item.data[-1];item.data[-1]=static_cast<unsigned char>(row);
                unsigned long expectedSize=item.owner->GetAllocationSize(item.data);
                long before=*reinterpret_cast<long*>(reinterpret_cast<unsigned char*>(&g_CMemoryAllocatorCriticalSection_013CAA70)+8);
                require(main->GetAllocationSize(item.data+4)==expectedSize);++sizeQueries;
                require(*reinterpret_cast<long*>(reinterpret_cast<unsigned char*>(&g_CMemoryAllocatorCriticalSection_013CAA70)+8)==before);
                g_FableAllocatorRangeStart_013CAAA4=reinterpret_cast<unsigned long>(item.data+4);g_FableAllocatorRangeEnd_013CAAA8=g_FableAllocatorRangeStart_013CAAA4+1;
                require(main->GetAllocationSize(item.data+4)==MainAccess::Table()[0].PAllocator->GetAllocationSize(item.data));++sizeQueries;
                g_FableAllocatorRangeStart_013CAAA4=g_FableAllocatorRangeEnd_013CAAA8=0;
                item.data[-1]=255;require(main->GetAllocationSize(item.data+4)==0);++sizeQueries;
                require(*reinterpret_cast<long*>(reinterpret_cast<unsigned char*>(&g_CMemoryAllocatorCriticalSection_013CAA70)+8)==before+1);LeaveCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
                require(main->GetAllocationSize(0)==0);++sizeQueries;require(*reinterpret_cast<long*>(reinterpret_cast<unsigned char*>(&g_CMemoryAllocatorCriticalSection_013CAA70)+8)==before+1);LeaveCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
                item.data[-1]=savedID;
#ifdef FABLE_NATIVE_MAIN_INIT
                if(expectedSize>=20){
                    unsigned char* saved=new unsigned char[expectedSize];unsigned char* predicted=new unsigned char[expectedSize];memcpy(saved,item.data,expectedSize);memcpy(predicted,saved,expectedSize);
                    unsigned char ownerExpected[sizeof(CMemoryAllocator)];reinterpret_cast<unsigned char*>(main)[0x414]=static_cast<unsigned char>(row);memcpy(ownerExpected,main,sizeof(ownerExpected));
                    *reinterpret_cast<unsigned long*>(predicted)=0xffffffff;unsigned long* metadata=reinterpret_cast<unsigned long*>(predicted+expectedSize-16);metadata[0]=0xffffffff;metadata[1]=row;metadata[2]=item.size;
                    *reinterpret_cast<unsigned long*>(ownerExpected+row*4)+=item.size;
                    item.data[-1]=static_cast<unsigned char>(row);MainAccess::Initialize(main,item.data+4,i%2!=0,item.size);
                    require(memcmp(predicted,item.data,expectedSize)==0&&memcmp(ownerExpected,main,sizeof(ownerExpected))==0);
                    require(*reinterpret_cast<long*>(reinterpret_cast<unsigned char*>(&g_CMemoryAllocatorCriticalSection_013CAA70)+8)==before);
                    memcpy(item.data,saved,expectedSize);item.data[-1]=savedID;delete[] predicted;delete[] saved;++metadataChecks;
                }
#endif
#endif
            }
            if(leaveLive&&cycle==1)break;
            while(liveCount){unsigned long chosen=order==0?0:order==1?liveCount-1:liveCount/2;live[chosen].owner->Deallocate(live[chosen].data);for(unsigned long i=chosen+1;i<liveCount;++i)live[i-1]=live[i];--liveCount;++operations;verify(main);}
        }
        void* chunks[128];unsigned long kinds[128],chunkCount=0;
        for(unsigned long row=0;row<30;++row){
            if(row<28){CMemoryAllocatorFixedSize* owner=static_cast<CMemoryAllocatorFixedSize*>(MainAccess::Table()[row].PAllocator);for(CMemoryAllocatorFixedSize::CMemoryAreaChunk* c=owner->PFirstChunk;c;c=c->PNextChunk){require(chunkCount<128);chunks[chunkCount]=c->ThisChunkMem;kinds[chunkCount++]=0;}}
            else{CMemoryAllocatorVariableSize* owner=static_cast<CMemoryAllocatorVariableSize*>(MainAccess::Table()[row].PAllocator);for(CMemoryAllocatorVariableSize::CMemoryAreaChunk* c=owner->PFirstChunk;c;c=c->PNextChunk){require(chunkCount<128);chunks[chunkCount]=c;kinds[chunkCount++]=1;}}
        }
        // Direct size-class calls do not increment main NoAllocations. Explicitly
        // set its documented counter to exercise the deferred shutdown branch.
        *reinterpret_cast<unsigned long*>(reinterpret_cast<unsigned char*>(main)+0x40c)=1;
        FableMemoryAllocatorShutdown_009f2d20();require(g_FableMemoryAllocator_013CAA90==main&&g_FableAllocatorShutdownRequested_013CAA94);verify(main);
        *reinterpret_cast<unsigned long*>(reinterpret_cast<unsigned char*>(main)+0x40c)=0;
        FableMemoryAllocatorShutdown_009f2d20();require(g_FableMemoryAllocator_013CAA90==0&&state(main)==0x10000);
        for(unsigned long row=0;row<30;++row)require(state(objects[row])==0x10000);
        for(unsigned long i=0;i<liveCount;++i)for(unsigned long j=0;j<live[i].size;++j)require(live[i].data[j]==live[i].tag);
        for(unsigned long i=0;i<chunkCount;++i){++retained;if(kinds[i]){require(state(chunks[i])==0x1000);require(VirtualFree(chunks[i],0,0x8000)!=0);}else{require(HeapValidate(GetProcessHeap(),0,chunks[i])!=0);require(HeapFree(GetProcessHeap(),1,chunks[i])!=0);}}
        require(HeapValidate(GetProcessHeap(),0,0)!=0);FableMemoryAllocatorShutdown_009f2d20();require(g_FableMemoryAllocator_013CAA90==0);++scenario;
    }
    printf("MAIN_ALLOCATOR_OS_OK scenarios=%lu operations=%lu checks=%lu retained_chunks=%lu\n",scenario,operations,checks,retained);
#ifdef FABLE_NATIVE_MAIN_SIZE
    require(sizeQueries==4320);printf("MAIN_SIZE_OS_QUERIES_OK queries=%lu\n",sizeQueries);
#ifdef FABLE_NATIVE_MAIN_INIT
    printf("MAIN_INIT_OS_METADATA_OK checks=%lu\n",metadataChecks);
#endif
#endif
    return 0;
}
