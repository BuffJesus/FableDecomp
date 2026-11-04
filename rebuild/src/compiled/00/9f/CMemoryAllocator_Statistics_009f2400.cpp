// lib_memory_allocator.cpp statistics, retail 0x009F2400-0x009F2826 (Ego_r
// order; GetTotalMemoryAllocated 0x009F2450 and GetNoAllocatedAreas 0x009F2550
// are their own TUs): each sums one size-class allocator query under the
// shared lock.
#include "fable_memory_allocator.h"

// Retail 009F2400.
unsigned long CMemoryAllocator::GetTotalMemoryControlledByAllocator()
{
    unsigned long total = 0;
    EnterSynchronisation();
    for (unsigned long i = 0; i < NoAllocators; ++i)
        total += AllocatorInfo[i].PAllocator->GetTotalMemoryControlledByAllocator();
    LeaveSynchronisation();
    return total;
}

// Retail 009F24A0: only the fixed-size classes (variable-size ones have -1).
unsigned long CMemoryAllocator::GetTotalFixedSizeMemoryAllocated()
{
    unsigned long total = 0;
    EnterSynchronisation();
    for (unsigned long i = 0; i < NoAllocators; ++i)
    {
        if (AllocatorInfo[i].MaxAllocSize > -1)
            total += AllocatorInfo[i].PAllocator->GetTotalMemoryAllocated();
    }
    LeaveSynchronisation();
    return total;
}

// Retail 009F2500.
unsigned long CMemoryAllocator::GetNoFreeAreas()
{
    unsigned long total = 0;
    EnterSynchronisation();
    for (unsigned long i = 0; i < NoAllocators; ++i)
        total += AllocatorInfo[i].PAllocator->GetNoFreeAreas();
    LeaveSynchronisation();
    return total;
}

// Retail 009F25A0.
float CMemoryAllocator::GetFragmentation()
{
    float total = 0.0f;
    EnterSynchronisation();
    for (unsigned long i = 0; i < NoAllocators; ++i)
        total += AllocatorInfo[i].PAllocator->GetFragmentation();
    LeaveSynchronisation();
    return total;
}

// Retail 009F2610.
unsigned long CMemoryAllocator::GetHeaderOverhead()
{
    unsigned long total = 0;
    EnterSynchronisation();
    for (unsigned long i = 0; i < NoAllocators; ++i)
        total += AllocatorInfo[i].PAllocator->GetHeaderOverhead();
    LeaveSynchronisation();
    return total;
}

// Retail 009F27E0.
unsigned long CMemoryAllocator::GetTotalMemoryUsedWithTrackingID(unsigned char id)
{
    EnterSynchronisation();
    unsigned long total = MemoryAllocatedPerTrackingID[id];
    LeaveSynchronisation();
    return total;
}

// Retail 009F2810: the lock around InternalValidate, which is empty in this
// build (0x009F1F50) and expanded here, as in retail's single file.
void CMemoryAllocator::Validate()
{
    EnterSynchronisation();
    LeaveSynchronisation();
}

// Retail 009F2A00: asks for shutdown, and frees the allocator now if nothing
// is allocated (otherwise the last Deallocate does it).
void FABLE_FASTCALL CMemoryAllocator::Destroy()
{
    if (Get())
    {
        g_FableAllocatorShutdownRequested_013CAA94 = true;
        if (Get()->NoAllocations == 0)
        {
            delete Get();
            g_FableMemoryAllocator_013CAA90 = 0;
        }
    }
}
