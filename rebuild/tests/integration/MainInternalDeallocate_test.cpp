#include "fable_memory_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
struct _RTL_CRITICAL_SECTION{void* debug;long lockCount,recursionCount;void* thread;void* semaphore;unsigned long spin;};
_RTL_CRITICAL_SECTION g_CMemoryAllocatorCriticalSection_013CAA70;
unsigned long g_FableAllocatorRangeStart_013CAAA4,g_FableAllocatorRangeEnd_013CAAA8;
CMemoryAllocator::CAllocatorInfo CMemoryAllocator::AllocatorInfo[30];
class Access:public CMemoryAllocator{public:using CMemoryAllocator::InternalDeallocate;static CAllocatorInfo* Table(){return AllocatorInfo;}};
static unsigned char* const region=reinterpret_cast<unsigned char*>(0x2004000);
static FILE* input;static unsigned long args[6],calls,depth,queries;static unsigned char expected[0x8000],snapshot[0x8000],actual[0x8000];
static unsigned long __fastcall QueryRecording(void*,void*,void*);
static void __fastcall FreeRecording(void*,void*,void*);
static void __fastcall FreeAlternate(void*,void*,void*);
static void read(void* p,unsigned long n){if(fread(p,1,n,input)!=n)exit(2);}
static void canonical()
{
    memcpy(actual,region,sizeof(actual));memcpy(actual+0x2000,Access::Table(),840);memcpy(actual+0x3000,&g_FableAllocatorRangeStart_013CAAA4,4);memcpy(actual+0x3004,&g_FableAllocatorRangeEnd_013CAAA8,4);
    if(*reinterpret_cast<unsigned long*>(actual+0x180c)!=reinterpret_cast<unsigned long>(QueryRecording))exit(7);
    *reinterpret_cast<unsigned long*>(actual+0x180c)=0x200d100;
    if(*reinterpret_cast<unsigned long*>(actual+0x190c)!=reinterpret_cast<unsigned long>(QueryRecording)||*reinterpret_cast<unsigned long*>(actual+0x1804)!=reinterpret_cast<unsigned long>(FreeRecording)||*reinterpret_cast<unsigned long*>(actual+0x1904)!=reinterpret_cast<unsigned long>(FreeAlternate))exit(7);
    *reinterpret_cast<unsigned long*>(actual+0x190c)=0x200d100;*reinterpret_cast<unsigned long*>(actual+0x1804)=0x200d400;*reinterpret_cast<unsigned long*>(actual+0x1904)=0x200d500;
}
static void boundary(unsigned long kind,unsigned long owner,unsigned long pointer)
{
    if(calls>=args[5])exit(8);unsigned long event[4];read(event,sizeof(event));read(snapshot,sizeof(snapshot));canonical();
    if(event[0]!=kind||event[1]!=owner||event[2]!=pointer||event[3]!=depth||memcmp(actual,snapshot,sizeof(snapshot)))exit(9);++calls;
}
static void __stdcall EnterRecording(_RTL_CRITICAL_SECTION* lock)
{if(lock!=&g_CMemoryAllocatorCriticalSection_013CAA70)exit(10);boundary(1,0,0);++depth;}
static unsigned long __fastcall QueryRecording(void* owner,void*,void* allocation)
{
    boundary(2,reinterpret_cast<unsigned long>(owner),reinterpret_cast<unsigned long>(allocation));
    if(args[1]==1&&queries==0){Access::Table()[args[3]].PAllocator=reinterpret_cast<CMemoryAllocatorBase*>(region+0x1400);region[0x414]=99;}
    if(args[1]==1&&queries==2)*reinterpret_cast<unsigned long*>(args[4])=0x2005900;
    return queries++==0?0:queries==2?256:args[2];
}
static void release(unsigned long kind,void* owner,void* allocation)
{boundary(kind,reinterpret_cast<unsigned long>(owner),reinterpret_cast<unsigned long>(allocation));if(args[1]==1){*reinterpret_cast<unsigned long*>(region+0x40c)=0;region[0x414]=77;}}
static void __fastcall FreeRecording(void* owner,void*,void* allocation){release(5,owner,allocation);}
static void __fastcall FreeAlternate(void* owner,void*,void* allocation){release(6,owner,allocation);}
static void __stdcall LeaveRecording(_RTL_CRITICAL_SECTION* lock)
{if(lock!=&g_CMemoryAllocatorCriticalSection_013CAA70)exit(10);boundary(3,0,0);--depth;}
extern "C"{void(__stdcall* EnterSlot)(_RTL_CRITICAL_SECTION*)=EnterRecording;void(__stdcall* LeaveSlot)(_RTL_CRITICAL_SECTION*)=LeaveRecording;}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;input=fopen(argv[1],"rb");if(!input)return 2;
    if(VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4)!=reinterpret_cast<void*>(0x2000000))return 3;
    unsigned long cases;read(&cases,4);
    for(unsigned long i=0;i<cases;++i){
        read(args,sizeof(args));read(region,sizeof(expected));read(expected,sizeof(expected));memcpy(Access::Table(),region+0x2000,840);memcpy(&g_FableAllocatorRangeStart_013CAAA4,region+0x3000,4);memcpy(&g_FableAllocatorRangeEnd_013CAAA8,region+0x3004,4);
        *reinterpret_cast<unsigned long*>(region+0x180c)=*reinterpret_cast<unsigned long*>(region+0x190c)=reinterpret_cast<unsigned long>(QueryRecording);*reinterpret_cast<unsigned long*>(region+0x1804)=reinterpret_cast<unsigned long>(FreeRecording);*reinterpret_cast<unsigned long*>(region+0x1904)=reinterpret_cast<unsigned long>(FreeAlternate);calls=depth=queries=0;
        reinterpret_cast<Access*>(region)->InternalDeallocate(reinterpret_cast<void*>(args[0]));canonical();
        if(queries!=(args[1]==2?0:3)||depth!=0||calls!=args[5]||memcmp(actual,expected,sizeof(expected))){printf("main free case %lu failed\n",i);return 4;}
    }
    if(fgetc(input)!=EOF)return 5;printf("MAIN_INTERNAL_DEALLOCATE_OK cases=%lu\n",cases);return 0;
}
