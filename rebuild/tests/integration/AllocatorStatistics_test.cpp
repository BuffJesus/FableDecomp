#include "fable_memory_allocator.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Real Win32 critical section (layout of RTL_CRITICAL_SECTION) so the imported
// Enter/LeaveCriticalSection run for real; RecursionCount (+0x08) tells whether
// the calling thread holds the lock.
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION*);
static long Held() { return g_CMemoryAllocatorCriticalSection_013CAA70.RecursionCount; }

static char g_log[64]; static int g_n;
static unsigned long* g_count; static int g_mode, g_slot, g_initial;
static unsigned long Record(int index, int slot) { if(slot!=g_slot || Held()!=g_initial+1) { puts("FAIL slot/lock"); exit(1); } g_log[g_n++]=(char)index; if(index==0 && (g_mode==4 || g_mode==5)) *g_count=g_mode==4?3:1; return g_mode==6?0xfffffff0UL+index*17:(index+1)*(slot==0?19:7); }
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
    unsigned long GetTotalMemoryAllocated() { return Record(Tag,0); }
    unsigned long GetNoFreeAreas() { return 0; }
    unsigned long GetNoAllocatedAreas() { return Record(Tag,1); }
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

// RETAIL_EXPECTATIONS
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION*);
int main()
{
    InitializeCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
    CRecordingAllocator allocators[30];
    for(int i=0;i<30;++i) { allocators[i].Tag=(char)i; Peek::Info()[i].PAllocator=&allocators[i]; }
    static unsigned long storage[0x420/4];
    CMemoryAllocator* owner=reinterpret_cast<CMemoryAllocator*>(storage);
    g_count=reinterpret_cast<unsigned long*>(reinterpret_cast<unsigned char*>(storage)+0x410);
    static const unsigned long counts[9]={0,1,3,15,1,3,3,3,30};
    for(g_mode=0;g_mode<9;++g_mode) for(g_slot=0;g_slot<2;++g_slot) {
        *g_count=counts[g_mode]; g_n=0; g_initial=g_mode==7?1:0;
        if(g_initial) EnterCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
        unsigned long result=g_slot==0?owner->GetTotalMemoryAllocated():owner->GetNoAllocatedAreas();
        int row=g_mode*2+g_slot;
        if(result!=expected[row][0] || (unsigned long)g_n!=expected[row][1] || *g_count!=expected[row][2] || Held()!=g_initial) { printf("FAIL row=%d result=%lu calls=%d count=%lu held=%ld\n",row,result,g_n,*g_count,Held()); return 1; }
        for(int i=0;i<g_n;++i) if(g_log[i]!=i) { puts("FAIL order");return 2; }
        if(g_initial) LeaveCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
    }
    DeleteCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
    puts("ALLOCATOR_STATISTICS_OK cases=18");return 0;
}
