// Native size-class table at 0x01396868 and zero-filled lock at 0x013CAA70.
// The 30 allocator rows are distinct from the 15 tracking categories.
#include "fable_memory_allocator.h"
struct _RTL_CRITICAL_SECTION {
    void* DebugInfo; long LockCount; long RecursionCount;
    void* OwningThread; void* LockSemaphore; unsigned long SpinCount;
};

CMemoryAllocator::CAllocatorInfo CMemoryAllocator::AllocatorInfo[30] = {
    {4,2048,0,4,0,3,0}, {8,2048,0,4,0,3,0},
    {12,4096,0,4,0,3,0}, {16,4096,0,4,0,3,0},
    {20,4096,0,4,0,3,0}, {24,4096,0,4,0,3,0},
    {32,8192,0,4,0,3,0}, {36,8192,0,4,0,3,0},
    {40,8192,0,4,0,3,0}, {48,8192,0,4,0,3,0},
    {56,8192,0,4,0,3,0}, {64,8192,0,4,0,3,0},
    {72,8192,0,4,0,3,0}, {80,8192,0,4,0,3,0},
    {88,8192,0,4,0,3,0}, {96,8192,0,4,0,3,0},
    {104,8192,0,4,0,3,0}, {112,8192,0,4,0,3,0},
    {120,8192,0,4,0,3,0}, {128,8192,0,4,0,3,0},
    {144,16384,0,4,0,3,0}, {160,16384,0,4,0,3,0},
    {176,16384,0,4,0,3,0}, {192,16384,0,4,0,3,0},
    {208,16384,0,4,0,3,0}, {224,16384,0,4,0,3,0},
    {240,16384,0,4,0,3,0}, {256,16384,0,4,0,3,0},
    {-1,1048576,0,16,0,4,0}, {-1,1048576,0,16,9,4,0}
};

_RTL_CRITICAL_SECTION g_CMemoryAllocatorCriticalSection_013CAA70;
unsigned long CMemoryAllocator::MemoryUseBeforeAllocator;
unsigned long g_FableAllocatorMemoryBaseline_013CAAB0;
bool g_FableAllocatorShutdownRequested_013CAA94;
