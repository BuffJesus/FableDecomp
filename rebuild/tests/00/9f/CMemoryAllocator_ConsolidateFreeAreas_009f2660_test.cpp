// Behaviour: ConsolidateFreeAreas calls slot 13 on AllocatorInfo[0..NoAllocators-1]
// .PAllocator, in order, each while the allocator critical section is held, and
// releases it afterwards; NoAllocators is read from +0x410 of `this`.
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
    CRecordingAllocator a, b, c;
    a.Tag = 'a'; b.Tag = 'b'; c.Tag = 'c';
    Peek::Info()[0].PAllocator = &a; Peek::Info()[1].PAllocator = &b; Peek::Info()[2].PAllocator = &c;
    static unsigned char storage[0x420];
    CMemoryAllocator* allocator = reinterpret_cast<CMemoryAllocator*>(storage);

    *reinterpret_cast<unsigned long*>(storage + 0x410) = 0;
    g_n = 0; memset(g_log, 0, sizeof(g_log));
    allocator->ConsolidateFreeAreas();
    if (g_n != 0 || Held() != 0) { printf("FAIL none\n"); return 1; }

    *reinterpret_cast<unsigned long*>(storage + 0x410) = 2;
    a.HeldDuringCall = b.HeldDuringCall = c.HeldDuringCall = -1;
    allocator->ConsolidateFreeAreas();
    if (strcmp(g_log, "ab") != 0 || a.HeldDuringCall != 1 || b.HeldDuringCall != 1 || c.HeldDuringCall != -1 || Held() != 0)
    { printf("FAIL two %s\n", g_log); return 2; }

    g_n = 0; memset(g_log, 0, sizeof(g_log));
    *reinterpret_cast<unsigned long*>(storage + 0x410) = 3;
    allocator->ConsolidateFreeAreas();
    if (strcmp(g_log, "abc") != 0 || c.HeldDuringCall != 1 || Held() != 0) { printf("FAIL three %s\n", g_log); return 3; }
    printf("FABLETLC_MEMALLOC_CONSOLIDATE PASS\n");
    return 0;
}