#include "fable_memory_allocator.h"
struct _RTL_CRITICAL_SECTION{void* debug;long LockCount,RecursionCount;void* thread;void* semaphore;unsigned long spin;};
struct EXCEPTION_RECORD{unsigned long ExceptionCode,ExceptionFlags;EXCEPTION_RECORD* next;void* address;unsigned long count;unsigned long ExceptionInformation[15];};
struct EXCEPTION_POINTERS{EXCEPTION_RECORD* ExceptionRecord;void* context;};
extern "C" void* __cdecl _exception_info();
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION*);
#define EXCEPTION_CONTINUE_SEARCH 0
#define EXCEPTION_EXECUTE_HANDLER 1
#define GetExceptionInformation() ((EXCEPTION_POINTERS*)_exception_info())
#include <stdio.h>
#include <stdlib.h>
_RTL_CRITICAL_SECTION g_CMemoryAllocatorCriticalSection_013CAA70;
CMemoryAllocator* g_FableMemoryAllocator_013CAA90;
bool g_FableAllocatorShutdownRequested_013CAA94;
static volatile unsigned long calls,requested,alignment,created,faultAddress,faultKind;
void FABLE_FASTCALL CMemoryAllocator::Create(){++created;g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)0x2004000;}
void* CMemoryAllocator::InternalAllocate(long size,unsigned long category,bool debug){if((unsigned long)size!=requested+alignment+3||category||debug||g_CMemoryAllocatorCriticalSection_013CAA70.RecursionCount!=1)exit(7);++calls;return 0;}
void CMemoryAllocator::InternalDeallocate(void*){exit(8);}
CMemoryAllocator::~CMemoryAllocator(){exit(8);}
static int filter(EXCEPTION_POINTERS* p){if(p->ExceptionRecord->ExceptionCode!=0xc0000005)return EXCEPTION_CONTINUE_SEARCH;faultKind=(unsigned long)p->ExceptionRecord->ExceptionInformation[0];faultAddress=(unsigned long)p->ExceptionRecord->ExceptionInformation[1];return EXCEPTION_EXECUTE_HANDLER;}
int main(int argc,char** argv){if(argc!=2)return 2;FILE* f=fopen(argv[1],"rb");unsigned long cases;if(!f||fread(&cases,4,1,f)!=1)return 2;InitializeCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);for(unsigned long i=0;i<cases;++i){unsigned long row[4];if(fread(row,4,4,f)!=4)return 2;requested=row[0];alignment=row[1];calls=created=0;faultAddress=faultKind=0xffffffff;g_FableMemoryAllocator_013CAA90=row[2]?(CMemoryAllocator*)0x2004000:0;__try{CMemoryAllocator::AllocateAligned(requested,alignment);return 3;}__except(filter(GetExceptionInformation())){}if(calls!=1||created!=(row[2]?0UL:1UL)||faultKind!=1||faultAddress!=row[3]||g_CMemoryAllocatorCriticalSection_013CAA70.RecursionCount!=0){printf("case=%lu calls=%lu created=%lu kind=%lu address=%lu expected=%lu depth=%ld\n",i,calls,created,faultKind,faultAddress,row[3],g_CMemoryAllocatorCriticalSection_013CAA70.RecursionCount);return 4;}}DeleteCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);printf("MAIN_ALIGNED_FAILURE_OK cases=%lu\n",cases);return 0;}


