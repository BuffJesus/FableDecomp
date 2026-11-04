#include "fable_memory_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
struct _RTL_CRITICAL_SECTION{void* debug;long lockCount,recursionCount;void* thread;void* semaphore;unsigned long spin;};
_RTL_CRITICAL_SECTION g_CMemoryAllocatorCriticalSection_013CAA70;
CMemoryAllocator* g_FableMemoryAllocator_013CAA90;
static unsigned char* const region=(unsigned char*)0x2004000;
static FILE* input;static unsigned long args[6],calls,depth;static unsigned char expected[0x8000],snapshot[0x8000],actual[0x8000];
static void read(void* p,unsigned long n){if(fread(p,1,n,input)!=n)exit(2);}
static void canonical(){memcpy(actual,region,sizeof(actual));memcpy(actual+0x3000,&g_FableMemoryAllocator_013CAA90,4);memcpy(actual+0x3004,&depth,4);}
static void boundary(unsigned long kind,unsigned long a=0,unsigned long b=0,unsigned long c=0){unsigned long event[4];if(calls>=args[5])exit(8);read(event,sizeof(event));read(snapshot,sizeof(snapshot));canonical();if(event[0]!=kind||event[1]!=a||event[2]!=b||event[3]!=c||memcmp(actual,snapshot,sizeof(snapshot)))exit(9);++calls;}
static void __stdcall EnterRecording(_RTL_CRITICAL_SECTION* lock){if(lock!=&g_CMemoryAllocatorCriticalSection_013CAA70)exit(10);boundary(1);++depth;if(args[3]==1)g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)(region+0x1000);}
static void __stdcall LeaveRecording(_RTL_CRITICAL_SECTION* lock){if(lock!=&g_CMemoryAllocatorCriticalSection_013CAA70)exit(10);boundary(2);--depth;if(args[3]==2)g_FableMemoryAllocator_013CAA90=0;}
extern "C"{void(__stdcall* EnterSlot)(_RTL_CRITICAL_SECTION*)=EnterRecording;void(__stdcall* LeaveSlot)(_RTL_CRITICAL_SECTION*)=LeaveRecording;}
void FABLE_FASTCALL CMemoryAllocator::Create(){boundary(4);g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)region;}
void* CMemoryAllocator::InternalAllocate(long size,unsigned long category,const char* file,long line,const char* function){if((unsigned long)file!=0xdeadbeef||line!=-1||(unsigned long)function!=0xbaadf00d)exit(11);boundary(5,(unsigned long)this,(unsigned long)size,category);if(args[3]==2)g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)(region+0x1000);return(void*)args[4];}
int main(int argc,char** argv){if(argc!=2)return 2;input=fopen(argv[1],"rb");if(!input)return 2;if(VirtualAlloc((void*)0x2000000,0x20000,0x3000,4)!=(void*)0x2000000)return 3;unsigned long cases;read(&cases,4);for(unsigned long i=0;i<cases;++i){read(args,sizeof(args));read(region,sizeof(expected));read(expected,sizeof(expected));memcpy(&g_FableMemoryAllocator_013CAA90,region+0x3000,4);calls=depth=0;void* result=CMemoryAllocator::Allocate(args[0],args[1],(const char*)0xdeadbeef,0xffffffff,(const char*)0xbaadf00d);canonical();if((unsigned long)result!=args[4]||depth||calls!=args[5]||memcmp(actual,expected,sizeof(expected)))return 4;}if(fgetc(input)!=EOF)return 5;printf("MAIN_DEBUG_STATIC_OK cases=%lu\n",cases);return 0;}
