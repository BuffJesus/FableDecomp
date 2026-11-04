// lib_handle_based_memory_pool.cpp (Ego_r CHandleBasedMemoryPool), retail
// 0x00A01D20-0x00A026A8 in Ego_r's function order: area headers, the
// free-list searches, the growable-area test, the pool's own critical
// section, initialisation and shutdown. One TU, because retail expands the
// small members (CreateArea, BeginCritical, ClearFreeLists, ...) into their
// callers here. Free lists are bucketed by size in FREE_LIST_SIZE_STEP steps,
// the last bucket holding everything larger.
// Retail calls the allocator's synchronisation and delete out of line here.
#define FABLE_MEMORY_ALLOCATOR_OUT_OF_LINE_SUPPORT
#define FABLE_HANDLE_POOL_HEADER_OUT_OF_LINE
#include "fable_handle_memory_pool.h"
#include "fable_memory_allocator_deallocate.h"
#include "fable_failed_allocation_handler.h"
#include "fable_random.h"
#include "fable_stlport_vector_impl.h"
#include <new>
#include <string.h>

// Retail 00A01D20: the handle is left for the allocator to set.
CHandleBasedMemoryPool::CHeader::CHeader()
    : Resource(0), Size(0), Padding(0), Status(STATUS_NULL), Locked(0),
      RefAvailable(0), NextAvailable(0), PrevContiguous(0), NextContiguous(0)
{
}

// Retail 00A01D40: the header sits directly in front of the data.
long CHandleBasedMemoryPool::GetHandleFromPointer(void* pointer)
{
    return ((CHeader*)pointer - 1)->Handle;
}

// Retail 00A01D50: a new free area spanning the buffer, linked between its
// contiguous neighbours.
CHandleBasedMemoryPool::CHeader* CHandleBasedMemoryPool::CreateArea(void* buffer, unsigned long size, CHeader* prev, CHeader* next)
{
    CHeader* header = (CHeader*)buffer;
    header->Size = size - sizeof(CHeader);
    header->Padding = 0;
    header->Status = STATUS_FREE;
    header->Locked = 0;
    header->PrevContiguous = prev;
    header->NextContiguous = next;
    header->Resource = 0;
    if (prev)
        prev->NextContiguous = header;
    if (next)
        next->PrevContiguous = header;
    return header;
}

// Retail 00A01ED0: the smallest unlocked area that fits, searching upward
// from the size's own bucket. The bucket clamp is a ternary here, not
// GetFreeListIndex's if: retail threads the clamped arm into the loop.
CHandleBasedMemoryPool::CHeader* CHandleBasedMemoryPool::FindFreeArea(long size)
{
    CHeader* best = 0;
    long best_size = 0x7fffffff;
    for (unsigned long index = (unsigned long)size / FREE_LIST_SIZE_STEP < FREE_LIST_COUNT - 1
             ? (unsigned long)size / FREE_LIST_SIZE_STEP : FREE_LIST_COUNT - 1;
         index < FREE_LIST_COUNT && !best; ++index)
    {
        for (CHeader* header = FreeLists[index]; header; header = header->NextAvailable)
        {
            if (size <= header->Size && header->Size < best_size && !header->Locked)
            {
                best_size = header->Size;
                best = header;
                if (best_size == size)
                    break;
            }
        }
    }
    return best;
}

// Retail 00A01F50.
CHandleBasedMemoryPool::CHeader* CHandleBasedMemoryPool::FindLargestFreeArea()
{
    unsigned long index = (unsigned long)LargestFreeArea / FREE_LIST_SIZE_STEP;
    if (index >= FREE_LIST_COUNT - 1)
        index = FREE_LIST_COUNT - 1;
    for (CHeader* header = FreeLists[index]; header; header = header->NextAvailable)
    {
        if (header->Size == LargestFreeArea)
            return header;
    }
    return 0;
}

// Retail 00A01F80: rescans downward from the old largest area's bucket.
void CHandleBasedMemoryPool::UpdateLargestFreeArea()
{
    for (long index = GetFreeListIndex(LargestFreeArea); index >= 0; --index)
    {
        if (FreeLists[index])
        {
            long largest = 0;
            for (CHeader* header = FreeLists[index]; header; header = header->NextAvailable)
            {
                if (largest < header->Size)
                    largest = header->Size;
            }
            LargestFreeArea = largest;
            return;
        }
    }
    LargestFreeArea = 0;
}

// Retail 00A01FE0: the check against LargestFreeArea is compiled out.
void CHandleBasedMemoryPool::ValidateLargestFreeArea()
{
    for (long index = FREE_LIST_COUNT - 1; index >= 0; --index)
    {
        if (FreeLists[index])
        {
            long largest = 0;
            for (CHeader* header = FreeLists[index]; header; header = header->NextAvailable)
            {
                if (largest < header->Size)
                    largest = header->Size;
            }
            return;
        }
    }
}

extern "C" __declspec(dllimport) int __stdcall TryEnterCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION*);

// Retail 00A02020: a free area can grow to `size` by absorbing its free or
// movable neighbours, first backwards, then forwards from the earliest start.
bool CHandleBasedMemoryPool::TestIfAreaIsGrowable(CHeader* header, long size)
{
    if (header->Status != STATUS_FREE)
        return false;
    char* start = (char*)(header + 1);
    char* end = start + header->Padding + header->Size;
    if (end - start >= size)
        return true;
    CHeader* area;
    for (area = header->PrevContiguous; area; area = area->PrevContiguous)
    {
        if (area->Status != STATUS_FREE && (area->Locked || !CanMoveResource(area->Resource, area->Handle)))
            break;
        start = (char*)(area + 1);
        if (end - start >= size)
            return true;
    }
    for (area = header->NextContiguous; area; area = area->NextContiguous)
    {
        if (area->Status != STATUS_FREE && (area->Locked || !CanMoveResource(area->Resource, area->Handle)))
            break;
        end = (char*)(area + 1) + area->Padding + area->Size;
        if (end - start >= size)
            return true;
    }
    return false;
}

// Retail 00A02100.
void CHandleBasedMemoryPool::ClearFreeLists()
{
    memset(FreeLists, 0, sizeof(FreeLists));
}

// Retail 00A02140: single-threaded pools never block.
bool CHandleBasedMemoryPool::BeginCritical(bool try_only)
{
    if (MultiThreaded)
    {
        if (try_only)
        {
            if (!TryEnterCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection)))
                return false;
        }
        else
            EnterCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection));
    }
    return true;
}

// Retail 00A02170.
void CHandleBasedMemoryPool::EndCritical()
{
    if (MultiThreaded)
        LeaveCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection));
}

// Retail 00A02190.
void CHandleBasedMemoryPool::Uninitialise()
{
    if (UnalignedBuffer)
    {
        CMemoryAllocator::Deallocate(UnalignedBuffer);
        UnalignedBuffer = 0;
    }
    Buffer = 0;
    PoolSize = 0;
    if (MultiThreaded)
    {
        DeleteCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection));
        MultiThreaded = false;
    }
    AllocatedSize = 0;
    NumAllocations = 0;
    LockCount = 0;
    LargestFreeArea = 0;
}

extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long);
double FABLE_FASTCALL GFGetTime();

// Retail 00A02220: waits up to `timeout` seconds for every allocation to be
// released (asking the failure handler to free some), then drops the buffer.
bool CHandleBasedMemoryPool::BeginShutdown(float timeout)
{
    BeginCritical(false);
    double start_time = GFGetTime();
    do
    {
        while (AllocatedSize > 0)
        {
            if (!FailureHandler || !FailureHandler->HandleFailedAllocation(AllocatedSize))
                break;
        }
        if (AllocatedSize == 0)
        {
            CMemoryAllocator::Deallocate(UnalignedBuffer);
            UnalignedBuffer = 0;
            Buffer = 0;
            ShutdownFlag = true;
            ClearFreeLists();
            EndCritical();
            return true;
        }
        EndCritical();
        Sleep(10);
        BeginCritical(false);
    } while (GFGetTime() <= start_time + timeout);
    EndCritical();
    return false;
}

// Retail 00A02370: reallocates the buffer dropped by BeginShutdown.
void CHandleBasedMemoryPool::EndShutdown()
{
    BeginCritical(false);
    if (ShutdownFlag)
    {
        ShutdownFlag = false;
        UnalignedBuffer = CMemoryAllocator::Allocate(PoolSize + DATA_ALIGNMENT, MemoryFlags);
        Buffer = (void*)(((unsigned long)UnalignedBuffer + DATA_ALIGNMENT - 1) / DATA_ALIGNMENT * DATA_ALIGNMENT);
        InsertFreeArea(CreateArea(Buffer, PoolSize, 0, 0));
    }
    EndCritical();
}

// Retail 00A02410: splits off the unused tail as a new free area when it can
// hold a header, otherwise keeps it as padding.
void CHandleBasedMemoryPool::ResizeArea(CHeader* header, long size)
{
    unsigned long spare = header->Size - size;
    if (spare > sizeof(CHeader))
    {
        InsertFreeArea(CreateArea((char*)(header + 1) + size, spare, header, header->NextContiguous));
        header->Size = size;
    }
    else
    {
        header->Size = size;
        header->Padding = (short)spare;
    }
}

// Retail 00A02470.
void CHandleBasedMemoryPool::CombineAdjacentAreas(CHeader*& header)
{
    if (header->NextContiguous && header->NextContiguous->Status == STATUS_FREE)
        CombineAreas(header, header->NextContiguous);
    CHeader* prev = header->PrevContiguous;
    if (prev && prev->Status == STATUS_FREE)
    {
        CombineAreas(prev, header);
        header = prev;
    }
}

// Retail 00A024C0: the per-area checks are compiled out; only the walks
// remain.
void CHandleBasedMemoryPool::Validate()
{
    BeginCritical(false);
    ValidateLargestFreeArea();
    CHeader* next;
    for (CHeader* area = (CHeader*)Buffer; area; area = next)
    {
        next = area->NextContiguous;
        for (long index = 0; index < FREE_LIST_COUNT; ++index)
        {
            for (CHeader* free_area = FreeLists[index]; free_area; free_area = free_area->NextAvailable)
            {
            }
        }
    }
    EndCritical();
}

// Retail 00A02570.
void CHandleBasedMemoryPool::Initialise(unsigned long size, bool multi_threaded, unsigned long memory_flags)
{
    Uninitialise();
    MultiThreaded = multi_threaded;
    if (multi_threaded)
        InitializeCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection));
    PoolSize = size;
    MemoryFlags = memory_flags;
    UnalignedBuffer = CMemoryAllocator::Allocate(PoolSize + DATA_ALIGNMENT, MemoryFlags);
    Buffer = (void*)(((unsigned long)UnalignedBuffer + DATA_ALIGNMENT - 1) / DATA_ALIGNMENT * DATA_ALIGNMENT);
    InsertFreeArea(CreateArea(Buffer, PoolSize, 0, 0));
}

// Retail 00A02610: the pool covers the aligned part of the caller's buffer.
// Near: retail adds buffer + size into the size register (6/38 lines).
void CHandleBasedMemoryPool::InitialiseFromPreallocatedBuffer(void* buffer, unsigned long size, bool multi_threaded)
{
    Uninitialise();
    MultiThreaded = multi_threaded;
    if (multi_threaded)
        InitializeCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection));
    Buffer = (void*)(((unsigned long)buffer + DATA_ALIGNMENT - 1) / DATA_ALIGNMENT * DATA_ALIGNMENT);
    PoolSize = ((unsigned long)buffer + size) / DATA_ALIGNMENT * DATA_ALIGNMENT - (unsigned long)Buffer;
    MemoryFlags = 0;
    InsertFreeArea(CreateArea(Buffer, PoolSize, 0, 0));
}

// Retail 00A02680.
bool CHandleBasedMemoryPool::ResizePool(unsigned long size, float timeout)
{
    if (BeginShutdown(timeout))
    {
        PoolSize = size;
        EndShutdown();
        return true;
    }
    return false;
}

// Retail 00A02730: slides every allocated area down to the start of the
// buffer, then turns the space left at the end into one free area. Areas
// already in place keep their header.
void CHandleBasedMemoryPool::Defragment()
{
    if (ShutdownFlag || !EnableDefrag)
        return;
    BeginCritical(false);
    if (LockCount <= 0)
    {
        CHeader* last = 0;
        long used = 0;
        CHeader* next;
        for (CHeader* area = (CHeader*)Buffer; area; area = next)
        {
            next = area->NextContiguous;
            if (area->RefAvailable)
                continue;
            CHeader* target = (CHeader*)((char*)Buffer + used);
            long size = area->Size;
            unsigned char locked = area->Locked;
            if (area != target || area->Padding != 0)
            {
                long handle = area->Handle;
                void* resource = area->Resource;
                Handles[handle].Header = 0;
                MoveResource(resource, handle, area + 1, target + 1, size);
                Handles[handle].Header = target;
                target->Size = size;
                target->Resource = resource;
                target->Handle = handle;
                target->Padding = 0;
                target->Status = STATUS_ALLOCATED;
                target->Locked = locked;
                target->RefAvailable = 0;
                target->NextAvailable = 0;
                target->PrevContiguous = last;
                target->NextContiguous = 0;
                if (last)
                    last->NextContiguous = target;
                last = target;
            }
            used += sizeof(CHeader) + size;
        }
        AllocatedSize = used;
        ClearFreeLists();
        unsigned long remaining = PoolSize - used;
        if (remaining > sizeof(CHeader))
            InsertFreeArea(CreateArea((char*)Buffer + used, remaining, last, 0));
    }
    EndCritical();
}

// Retail 00A028D0.
void* CHandleBasedMemoryPool::InternalGetResource(long handle)
{
    return Handles[handle].Header->Resource;
}

// Retail 00A028F0: moves an allocation into the free area `to`, which takes
// over its handle; `from` becomes free.
void CHandleBasedMemoryPool::MoveArea(CHeader* from, CHeader* to)
{
    long size = from->Size;
    MoveResource(from->Resource, from->Handle, from + 1, to + 1, size);
    RemoveFreeArea(to);
    ResizeArea(to, size);
    to->Resource = from->Resource;
    to->Status = from->Status;
    to->Handle = from->Handle;
    to->Locked = from->Locked;
    Handles[from->Handle].Header = to;
    AllocatedSize += to->Padding - from->Padding;
    from->Status = STATUS_FREE;
    InsertFreeArea(from);
}

// The test's own generator state (not named in Ego_r's public symbols).
extern unsigned long g_FableHandlePoolTestSeed_013CB504;

// Retail 00A029D0: a stress test that moves `count` random movable
// allocations into the best free area for them. Near (32/84 lines): retail
// keeps `this` in esi and the area in edi.
void CHandleBasedMemoryPool::RunRandomRelocationTest(long count)
{
    if (Handles.size() < 2)
        return;
    for (long test = count; test > 0; --test)
    {
        // Unused slots hold the next unused handle (or -1), never a pointer.
        CHandleInfo info = Handles[GFRandom(Handles.size(), &g_FableHandlePoolTestSeed_013CB504)];
        if (info.NextUnusedHandle == -1 || (unsigned long)info.NextUnusedHandle < Handles.size())
            continue;
        CHeader* area = info.Header;
        CHeader* free_area = FindFreeArea(area->Size);
        if (!free_area)
            continue;
        bool was_largest = free_area->Size == LargestFreeArea;
        if (!area->Locked && CanMoveResource(area->Resource, area->Handle))
        {
            MoveArea(area, free_area);
            CombineAdjacentAreas(area);
        }
        if (was_largest)
            UpdateLargestFreeArea();
    }
}

// Retail 00A02AD0.
void CHandleBasedMemoryPool::ValidateResources()
{
    for (unsigned long handle = 0; handle < Handles.size(); ++handle)
    {
        CHeader* area = Handles[handle].Header;
        if (area && area->Status == STATUS_ALLOCATED)
            ValidateResource(area->Resource);
    }
}

// Retail 00A02C00: grows a free area by one neighbour, next first: a free
// neighbour is merged, a movable one is moved out of the way (the area is
// locked meanwhile so the search cannot pick it).
bool CHandleBasedMemoryPool::GrowFreeArea(CHeader*& area)
{
    // `neighbour` is the copy CombineAdjacentAreas updates in place.
    if (CHeader* next = area->NextContiguous)
    {
        CHeader* neighbour = next;
        if (neighbour->RefAvailable)
        {
            CombineAreas(area, neighbour);
            return true;
        }
        if (!neighbour->Locked && CanMoveResource(neighbour->Resource, neighbour->Handle))
        {
            area->Locked = 1;
            CHeader* free_area = FindFreeArea(neighbour->Size);
            area->Locked = 0;
            if (free_area && free_area != neighbour && free_area != area)
            {
                MoveArea(neighbour, free_area);
                CombineAdjacentAreas(neighbour);
                area = neighbour;
                return true;
            }
        }
    }
    if (CHeader* prev = area->PrevContiguous)
    {
        CHeader* neighbour = prev;
        if (neighbour->RefAvailable)
        {
            CombineAreas(neighbour, area);
            area = neighbour;
            return true;
        }
        if (!neighbour->Locked && CanMoveResource(neighbour->Resource, neighbour->Handle))
        {
            area->Locked = 1;
            CHeader* free_area = FindFreeArea(neighbour->Size);
            area->Locked = 0;
            if (free_area && free_area != neighbour && free_area != area)
            {
                MoveArea(neighbour, free_area);
                CombineAdjacentAreas(neighbour);
                area = neighbour;
                return true;
            }
        }
    }
    return false;
}

// Retail 00A02D20.
CHandleBasedMemoryPool::CHandleBasedMemoryPool()
    : Buffer(0), UnalignedBuffer(0), PoolSize(0), AllocatedSize(0), NumAllocations(0), LockCount(0),
      MultiThreaded(false), ShutdownFlag(false), FailureHandler(0), FirstUnusedHandle(-1)
{
    ClearFreeLists();
    EnableDefrag = true;
    EnableFullDefragIfPartialFails = true;
    MaxFreeSizeBeforeDefrag = 0x80000;
    MaxFreeSizeBeforeFailure = 0x100000;
    MaxBlocksToTryOnPartialDefrag = 16;
}

// Retail 00A02DB0.
CHandleBasedMemoryPool::~CHandleBasedMemoryPool()
{
    Uninitialise();
}

// Retail 00A02DF0: first grows the largest free area if that can reach
// `size`; otherwise walks the free lists from the largest bucket down, growing
// up to MaxBlocksToTryOnPartialDefrag candidates and remembering the ones that
// got stuck.
bool CHandleBasedMemoryPool::PartialDefragment(long size)
{
    if (ShutdownFlag || !EnableDefrag)
        return false;
    BeginCritical(false);
    bool result = false;
    CHeader* largest = FindLargestFreeArea();
    if (largest && TestIfAreaIsGrowable(largest, size))
    {
        while (LargestFreeArea < size)
        {
            CHeader* area = FindLargestFreeArea();
            if (!area || !GrowFreeArea(area))
                break;
        }
    }
    if (LargestFreeArea >= size)
        result = true;
    else
    {
        long blocks_tried = 0;
        _STL::vector<CHeader*> stuck;
        for (long index = (unsigned long)LargestFreeArea / FREE_LIST_SIZE_STEP < FREE_LIST_COUNT - 1
                 ? (unsigned long)LargestFreeArea / FREE_LIST_SIZE_STEP : FREE_LIST_COUNT - 1;
             index >= 0 && !result && blocks_tried < MaxBlocksToTryOnPartialDefrag; --index)
        {
            CHeader* area = FreeLists[index];
            while (area && area->Size < size && blocks_tried < MaxBlocksToTryOnPartialDefrag)
            {
                bool already_stuck = false;
                for (unsigned long i = 0; i < stuck.size(); ++i)
                {
                    if (area == stuck[i])
                    {
                        already_stuck = true;
                        break;
                    }
                }
                CHeader* candidate = area;
                if (!already_stuck)
                {
                    ++blocks_tried;
                    if (TestIfAreaIsGrowable(area, size))
                    {
                        if (GrowFreeArea(area))
                        {
                            while (area->Size < size)
                            {
                                if (!GrowFreeArea(area))
                                {
                                    stuck.push_back(candidate);
                                    break;
                                }
                            }
                            if (area->Size >= size)
                            {
                                result = true;
                                break;
                            }
                            area = FreeLists[index];
                            continue;
                        }
                    }
                }
                area = area->NextAvailable;
            }
        }
        UpdateLargestFreeArea();
    }
    EndCritical();
    return result;
}

// Retail 00A03040: reuses the first unused handle, else appends an empty one
// (Ego_r has an inline CArray<CHandleInfo>::push_back() for this; retail
// stores the zero as a literal, which a value-initialised temporary does not).
long CHandleBasedMemoryPool::InternalAllocateHandle()
{
    if (FirstUnusedHandle != -1)
    {
        long handle = FirstUnusedHandle;
        CHandleInfo& info = Handles[handle];
        FirstUnusedHandle = info.NextUnusedHandle;
        info.NextUnusedHandle = -1;
        return handle;
    }
    long handle = Handles.size();
    CHandleInfo info;
    info.Header = 0;
    Handles.push_back(info);
    return handle;
}

// Retail 00A030D0: sizes are rounded up to DATA_ALIGNMENT.
bool CHandleBasedMemoryPool::InternalAllocate(long* handle, unsigned long size)
{
    size = (size + DATA_ALIGNMENT - 1) / DATA_ALIGNMENT * DATA_ALIGNMENT;
    CHeader* area = FindFreeArea(size);
    if (area)
    {
        bool was_largest = area->Size == LargestFreeArea;
        RemoveFreeArea(area);
        area->Status = STATUS_ALLOCATED;
        ResizeArea(area, size);
        ++NumAllocations;
        AllocatedSize += sizeof(CHeader) + area->Padding + area->Size;
        if (was_largest)
            UpdateLargestFreeArea();
        *handle = InternalAllocateHandle();
        Handles[*handle].Header = area;
        area->Handle = *handle;
        return true;
    }
    return false;
}

// Retail 00A03180: when the pool is short, the failure handler is asked to
// release memory, first down to MaxFreeSizeBeforeDefrag spare, then (after a
// partial defragment) down to MaxFreeSizeBeforeFailure. Flag 1: don't block
// on the lock; flag 2: don't call the failure handler.
bool CHandleBasedMemoryPool::Allocate(long* handle, unsigned long size, unsigned long flags)
{
    CScopedCritical critical(*this, (flags & 1) != 0);
    if (!critical.Entered)
        return false;
    if (ShutdownFlag)
        return false;
    // Retail tests the inverted bit as a bool (shr / not bl / test).
    if (FailureHandler && (bool)(~(flags >> 1) & 1))
    {
        while (size > (unsigned long)(PoolSize - AllocatedSize))
        {
            if (!FailureHandler->HandleFailedAllocation(size))
                return false;
        }
        while ((unsigned long)LargestFreeArea < size)
        {
            if ((unsigned long)(PoolSize - AllocatedSize) >= MaxFreeSizeBeforeDefrag + size
                || !FailureHandler->HandleFailedAllocation(size))
                break;
        }
        if ((unsigned long)LargestFreeArea < size && !PartialDefragment(size))
        {
            while ((unsigned long)LargestFreeArea < size)
            {
                if ((unsigned long)(PoolSize - AllocatedSize) >= MaxFreeSizeBeforeFailure + size
                    || !FailureHandler->HandleFailedAllocation(size))
                    break;
            }
            if ((unsigned long)LargestFreeArea < size && !PartialDefragment(size))
                return false;
        }
    }
    return InternalAllocate(handle, size);
}

// Retail 00A032F0.
bool CHandleBasedMemoryPool::Allocate(CScopedMemoryHandle& handle, unsigned long size, unsigned long flags)
{
    long new_handle;
    if (Allocate(&new_handle, size, flags))
    {
        handle.Initialise(this, new_handle);
        return true;
    }
    return false;
}

// Retail 00A034E0.
CHandleBasedMemoryPool::CScopedCritical::CScopedCritical(CHandleBasedMemoryPool& pool, bool try_only)
    : Pool(pool)
{
    Entered = pool.BeginCritical(try_only);
}

// Retail 00A03530.
CHandleBasedMemoryPool::CScopedCritical::~CScopedCritical()
{
    if (Entered)
        Pool.EndCritical();
}
