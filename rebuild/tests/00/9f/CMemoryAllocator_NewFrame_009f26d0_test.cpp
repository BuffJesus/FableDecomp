#include "fable_memory_allocator.h"
#include <stdio.h>
#include <string.h>

struct _RTL_CRITICAL_SECTION {
    void* DebugInfo; long LockCount; long RecursionCount;
    void* OwningThread; void* LockSemaphore; unsigned long SpinCount;
};
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION*);
_RTL_CRITICAL_SECTION g_CMemoryAllocatorCriticalSection_013CAA70;

int main()
{
    InitializeCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
    static unsigned long storage[0x420/4];
    static unsigned long before[0x420/4];
    memset(storage,0x77,sizeof(storage));
    memcpy(before,storage,sizeof(storage));
    for(int depth=0;depth<3;++depth) {
        for(int i=0;i<depth;++i) EnterCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
        reinterpret_cast<CMemoryAllocator*>(storage)->NewFrame();
        if(g_CMemoryAllocatorCriticalSection_013CAA70.RecursionCount!=depth || memcmp(storage,before,sizeof(storage))) { puts("FAIL NewFrame lock/state");return 1; }
        for(int i=0;i<depth;++i) LeaveCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
    }
    DeleteCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
    puts("NEW_FRAME_OK cases=3");return 0;
}
