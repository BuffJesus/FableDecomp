// Behaviour: ClearPerformanceStats takes and releases the allocator critical
// section (it must not be left held) and touches no allocator.
#include "fable_memory_allocator.h"
#include "../../../src/compiled/00/a6/CMemoryAllocatorBase_Constructor_00a65ad0.cpp"
#include <stdio.h>
#include <string.h>

// Real Win32 critical section (layout of RTL_CRITICAL_SECTION) so the imported
// Enter/LeaveCriticalSection run for real; RecursionCount (+0x08) tells whether
// the calling thread holds the lock.
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION*);
_RTL_CRITICAL_SECTION g_CMemoryAllocatorCriticalSection_013CAA70;
CMemoryAllocator::CAllocatorInfo CMemoryAllocator::AllocatorInfo[30];
static long Held() { return g_CMemoryAllocatorCriticalSection_013CAA70.RecursionCount; }

static char g_log[64]; static int g_n;
void CMemoryAllocatorBase::ConsolidateFreeAreas() { g_log[g_n++] = '?'; }  // base body: out of line in retail
class CRecordingAllocator : public CMemoryAllocatorBase
{
public:
    CRecordingAllocator() : CMemoryAllocatorBase(4) {}
    char Tag; long HeldDuringCall;
    void* Allocate(unsigned long) { return 0; }
    void Deallocate(void*) {}
    unsigned long GetAllAllocatedAreas(void**, unsigned long) { return 0; }
    unsigned long GetAllocationSize(void*) { return 0; }
    unsigned long GetAllocationSizeIncludingHeader(void*) { return 0; }
    unsigned long GetLargestFreeAreaSize() { return 0; }
    unsigned long GetTotalMemoryControlledByAllocator() { return 0; }
    unsigned long GetTotalMemoryAllocated() { return 0; }
    unsigned long GetNoFreeAreas() { return 0; }
    unsigned long GetNoAllocatedAreas() { return 0; }
    float GetFragmentation() { return 0; }
    void* GetAllocatedArea(long) { return 0; }
    unsigned long GetHeaderOverhead() { return 0; }
    void ConsolidateFreeAreas() { g_log[g_n++] = Tag; HeldDuringCall = Held(); }
    void Validate() {}
};

struct Peek : public CMemoryAllocator
{
    static CAllocatorInfo* Info() { return AllocatorInfo; }
};

int main()
{
    InitializeCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
    CRecordingAllocator a; a.Tag = 'a';
    Peek::Info()[0].PAllocator = &a;
    static unsigned char storage[0x420];
    static unsigned char before[0x420];
    memset(storage, 0x77, sizeof(storage));
    *reinterpret_cast<unsigned long*>(storage + 0x410) = 1;
    memcpy(before, storage, sizeof(storage));
    g_n = 0;
    reinterpret_cast<CMemoryAllocator*>(storage)->ClearPerformanceStats();
    if (g_n != 0 || Held() != 0 || memcmp(before, storage, sizeof(storage)) != 0) { printf("FAIL\n"); return 1; }
    // The lock must really have been released: a second call must not deadlock or nest.
    reinterpret_cast<CMemoryAllocator*>(storage)->ClearPerformanceStats();
    if (Held() != 0) { printf("FAIL nested\n"); return 2; }
    printf("009f26b0_TEST PASS\n");
    return 0;
}