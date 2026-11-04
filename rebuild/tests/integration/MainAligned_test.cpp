#include "fable_memory_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
struct _RTL_CRITICAL_SECTION{void* debug;long lockCount,recursionCount;void* thread;void* semaphore;unsigned long spin;};
_RTL_CRITICAL_SECTION g_CMemoryAllocatorCriticalSection_013CAA70;
CMemoryAllocator* g_FableMemoryAllocator_013CAA90;
bool g_FableAllocatorShutdownRequested_013CAA94;
static unsigned char* const region=(unsigned char*)0x2004000;
static FILE* input;static unsigned long args[11],calls,depth;static unsigned char expected[0x8000],snapshot[0x8000],actual[0x8000];
static void read(void* p,unsigned long n){if(fread(p,1,n,input)!=n)exit(2);}
static unsigned long& count(void* p){return *(unsigned long*)((unsigned char*)p+0x40c);}
static void canonical(){memcpy(actual,region,sizeof(actual));memcpy(actual+0x3000,&g_FableMemoryAllocator_013CAA90,4);memcpy(actual+0x3004,&g_FableAllocatorShutdownRequested_013CAA94,1);memcpy(actual+0x3008,&depth,4);}
static void boundary(unsigned long kind,unsigned long a=0,unsigned long b=0,unsigned long c=0){unsigned long event[4];if(calls>=args[10])exit(8);read(event,sizeof(event));read(snapshot,sizeof(snapshot));canonical();if(event[0]!=kind||event[1]!=a||event[2]!=b||event[3]!=c||memcmp(actual,snapshot,sizeof(snapshot))){printf("event %lu kind %lu failed\n",calls,kind);exit(9);}++calls;}
static void __stdcall EnterRecording(_RTL_CRITICAL_SECTION* lock){if(lock!=&g_CMemoryAllocatorCriticalSection_013CAA70)exit(10);boundary(1);++depth;if(args[7]==1){g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)(region+0x1000);if(args[0])*(unsigned long*)(args[1]-4)=args[3]+16;}else if(args[7]==2&&args[0])*(unsigned long*)(args[1]-4)=args[3]+16;}
static void __stdcall LeaveRecording(_RTL_CRITICAL_SECTION* lock){if(lock!=&g_CMemoryAllocatorCriticalSection_013CAA70)exit(10);boundary(2);if(!args[0]){unsigned long target=(args[3]+args[2]+3)&~(args[2]-1);if(args[7]==1)*(unsigned long*)(target-4)=0x12345678;else if(args[7]==2)g_FableMemoryAllocator_013CAA90=0;}--depth;}
static int __stdcall FreeRecording(void* p,unsigned long size,unsigned long flags){boundary(8,(unsigned long)p,size,flags);return args[8];}
extern "C"{void(__stdcall* EnterSlot)(_RTL_CRITICAL_SECTION*)=EnterRecording;void(__stdcall* LeaveSlot)(_RTL_CRITICAL_SECTION*)=LeaveRecording;int(__stdcall* FreeSlot)(void*,unsigned long,unsigned long)=FreeRecording;}
void FABLE_FASTCALL CMemoryAllocator::Create(){boundary(4);g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)(region+(args[7]==2?0x1000:0));}
void* CMemoryAllocator::InternalAllocate(long size,unsigned long category,bool debug){if(category||debug)exit(11);boundary(5,(unsigned long)this,(unsigned long)size);++count(this);if(args[7]==2)g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)region;return(void*)args[3];}
void CMemoryAllocator::InternalDeallocate(void* p){boundary(6,(unsigned long)this,(unsigned long)p);--count(this);if(args[7]==2){g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)(region+0x1000);count(g_FableMemoryAllocator_013CAA90)=0;g_FableAllocatorShutdownRequested_013CAA94=true;}}
CMemoryAllocator::~CMemoryAllocator(){boundary(7,(unsigned long)this);if(args[7]==2)g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)region;}
int main(int argc,char** argv){if(argc!=2)return 2;input=fopen(argv[1],"rb");if(!input)return 2;if(VirtualAlloc((void*)0x2000000,0x20000,0x3000,4)!=(void*)0x2000000)return 3;unsigned long cases;read(&cases,4);for(unsigned long i=0;i<cases;++i){read(args,sizeof(args));read(region,sizeof(expected));read(expected,sizeof(expected));memcpy(&g_FableMemoryAllocator_013CAA90,region+0x3000,4);memcpy(&g_FableAllocatorShutdownRequested_013CAA94,region+0x3004,1);calls=depth=0;unsigned long result=0;if(args[0])CMemoryAllocator::DeallocateAligned((void*)args[1]);else result=(unsigned long)CMemoryAllocator::AllocateAligned(args[1],args[2]);canonical();if(result!=args[9]||depth||calls!=args[10]||memcmp(actual,expected,sizeof(expected))){printf("aligned case %lu failed\n",i);return 4;}}if(fgetc(input)!=EOF)return 5;printf("MAIN_ALIGNED_OK cases=%lu\n",cases);return 0;}
