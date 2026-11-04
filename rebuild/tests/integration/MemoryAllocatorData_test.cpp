#include "fable_memory_allocator.h"
#include <stdio.h>
#include <string.h>
struct _RTL_CRITICAL_SECTION {
    void* DebugInfo; long LockCount; long RecursionCount;
    void* OwningThread; void* LockSemaphore; unsigned long SpinCount;
};
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION*);

struct View : CMemoryAllocator {
    static CAllocatorInfo* Info() { return AllocatorInfo; }
    static unsigned long Count() { return sizeof(AllocatorInfo)/sizeof(AllocatorInfo[0]); }
    static unsigned long& Baseline() { return MemoryUseBeforeAllocator; }
};
// RETAIL_EXPECTATIONS
int main()
{
    if(sizeof(CMemoryAllocator::CAllocatorInfo)!=28 || sizeof(_RTL_CRITICAL_SECTION)!=24 || View::Count()!=30) return 1;
    if(memcmp(View::Info(),expected,840)) return 2;
    unsigned char zero[24]={0};
    if(memcmp(&g_CMemoryAllocatorCriticalSection_013CAA70,zero,24)) return 3;
    if(View::Baseline() || g_FableAllocatorMemoryBaseline_013CAAB0 || g_FableAllocatorShutdownRequested_013CAA94)return 8;
    View::Baseline()=1234;g_FableAllocatorMemoryBaseline_013CAAB0=5678;g_FableAllocatorShutdownRequested_013CAA94=true;
    if(View::Baseline()!=1234 || g_FableAllocatorMemoryBaseline_013CAAB0!=5678 || !g_FableAllocatorShutdownRequested_013CAA94)return 9;
    View::Baseline()=g_FableAllocatorMemoryBaseline_013CAAB0=0;g_FableAllocatorShutdownRequested_013CAA94=false;
    for(int i=0;i<30;++i) {
        CMemoryAllocator::CAllocatorInfo before=View::Info()[i];
        View::Info()[i].MaxAllocSize=99;View::Info()[i].PAllocator=reinterpret_cast<CMemoryAllocatorBase*>(i+1);
        if(View::Info()[i].MaxAllocSize!=99 || View::Info()[i].PAllocator!=reinterpret_cast<CMemoryAllocatorBase*>(i+1))return 4;
        View::Info()[i]=before;
    }
    InitializeCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
    EnterCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
    EnterCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
    if(g_CMemoryAllocatorCriticalSection_013CAA70.RecursionCount!=2)return 5;
    LeaveCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
    LeaveCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
    if(g_CMemoryAllocatorCriticalSection_013CAA70.RecursionCount)return 6;
    DeleteCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
    if(memcmp(View::Info(),expected,840))return 7;
    puts("MEMORY_ALLOCATOR_DATA_OK rows=30");return 0;
}
