#include "fable_memory_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
struct _RTL_CRITICAL_SECTION{void* debug;long lockCount,recursionCount;void* thread;void* semaphore;unsigned long spin;};
_RTL_CRITICAL_SECTION g_CMemoryAllocatorCriticalSection_013CAA70;
unsigned long g_FableAllocatorRangeStart_013CAAA4,g_FableAllocatorRangeEnd_013CAAA8;
CMemoryAllocator::CAllocatorInfo CMemoryAllocator::AllocatorInfo[30];
class Access:public CMemoryAllocator{public:static CAllocatorInfo* Table(){return AllocatorInfo;}};
static unsigned char* const region=reinterpret_cast<unsigned char*>(0x2004000);
static FILE* input;static unsigned long args[7],calls,depth;static unsigned char expected[0x8000],snapshot[0x8000],actual[0x8000];
static unsigned long __fastcall QueryRecording(void*,void*,void*);
static void read(void* p,unsigned long n){if(fread(p,1,n,input)!=n)exit(2);}
static void canonical()
{
    memcpy(actual,region,sizeof(actual));memcpy(actual+0x2000,Access::Table(),840);memcpy(actual+0x3000,&g_FableAllocatorRangeStart_013CAAA4,4);memcpy(actual+0x3004,&g_FableAllocatorRangeEnd_013CAAA8,4);
    if(*reinterpret_cast<unsigned long*>(actual+0x180c)!=reinterpret_cast<unsigned long>(QueryRecording))exit(7);
    *reinterpret_cast<unsigned long*>(actual+0x180c)=0x200d100;
}
static void boundary(unsigned long kind,unsigned long owner,unsigned long pointer)
{
    if(calls>=args[6])exit(8);unsigned long event[4];read(event,sizeof(event));read(snapshot,sizeof(snapshot));canonical();
    if(event[0]!=kind||event[1]!=owner||event[2]!=pointer||event[3]!=depth||memcmp(actual,snapshot,sizeof(snapshot)))exit(9);++calls;
}
static void mutate(unsigned long which)
{
    if(which==1){*reinterpret_cast<unsigned long*>(region+0x410)=1;region[0x6000-5]=0;g_FableAllocatorRangeStart_013CAAA4=args[0];g_FableAllocatorRangeEnd_013CAAA8=args[0]+1;Access::Table()[0].PAllocator=reinterpret_cast<CMemoryAllocatorBase*>(region+0x1400);}
    else{*reinterpret_cast<unsigned long*>(region+0x410)=0;region[0x6000-5]=255;g_FableAllocatorRangeStart_013CAAA4=g_FableAllocatorRangeEnd_013CAAA8=0xffffffff;Access::Table()[0].PAllocator=0;}
}
static void __stdcall EnterRecording(_RTL_CRITICAL_SECTION* lock)
{if(lock!=&g_CMemoryAllocatorCriticalSection_013CAA70)exit(10);boundary(1,0,0);++depth;if(args[3]==1)mutate(1);}
static unsigned long __fastcall QueryRecording(void* owner,void*,void* allocation)
{boundary(2,reinterpret_cast<unsigned long>(owner),reinterpret_cast<unsigned long>(allocation));if(args[3]==2)mutate(2);return args[2];}
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
        *reinterpret_cast<unsigned long*>(region+0x180c)=reinterpret_cast<unsigned long>(QueryRecording);calls=0;depth=args[1];
        unsigned long result=reinterpret_cast<CMemoryAllocator*>(region)->GetAllocationSize(reinterpret_cast<void*>(args[0]));canonical();
        if(result!=args[4]||depth!=args[5]||calls!=args[6]||memcmp(actual,expected,sizeof(expected))){printf("main size case %lu failed\n",i);return 4;}
    }
    if(fgetc(input)!=EOF)return 5;printf("MAIN_ALLOCATION_SIZE_OK cases=%lu\n",cases);return 0;
}
