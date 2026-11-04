#pragma once
#include "fable_base_class.h"
#include "fable_carray.h"
#include "fable_string.h"
// Retail vtables 0129C964 / 0129D6A0 have four slots. PDB member names;
// retail STLport's 12-byte handle vector places the critical section at +44C.
class CFailedAllocationHandler;
class CMovableResource;
class CScopedMemoryHandle;
class CHandleBasedMemoryPool : public CBaseClassNonCopyable {
public:
    CHandleBasedMemoryPool();
    virtual ~CHandleBasedMemoryPool();
    // PDB EAreaStatus and the unnamed pool constants.
    enum EAreaStatus { STATUS_NULL = 0, STATUS_FREE = 78, STATUS_ALLOCATED = 84 };
    enum { DATA_ALIGNMENT = 16, FREE_LIST_SIZE_STEP = 512, FREE_LIST_COUNT = 256 };
    class CHeader;
    bool Allocate(long* handle, unsigned long size, unsigned long flags);
    bool Allocate(CScopedMemoryHandle& handle, unsigned long size, unsigned long flags);
    // Ego_r CHandleBasedMemoryPool::CScopedCritical (out of line in retail:
    // ctor 0x00A034E0, dtor 0x00A03530).
    class CScopedCritical {
    public:
        // VC71 needs inline on these declarations to merge caller-emitted
        // helper copies when the standalone pool provider is also linked.
        inline CScopedCritical(CHandleBasedMemoryPool& pool, bool try_only);
        inline ~CScopedCritical();
        CHandleBasedMemoryPool& Pool;
        bool Entered;
    };
    friend class CScopedCritical;
    void Free(long handle);
    void* GetPointerFromHandle(long);
    unsigned long GetSizeFromHandle(long handle); // Ego_r; retail 00A026D0.
    void Uninitialise();
    bool BeginShutdown(float timeout);
    void EndShutdown();
    void Validate();
    void Initialise(unsigned long size, bool multi_threaded, unsigned long memory_flags);
    void InitialiseFromPreallocatedBuffer(void* buffer, unsigned long size, bool multi_threaded);
    bool ResizePool(unsigned long size, float timeout);
    void Defragment();
    bool PartialDefragment(long size);
    void RunRandomRelocationTest(long count);
    void ValidateResources();
    void Lock(long handle);
    void Unlock(long handle);
    void InternalSetResource(long handle, void* resource);
    void* InternalGetResource(long handle);
    long GetHandleFromPointer(void* pointer);
    class CHeader {
    public:
        CHeader();
        void* Resource;
        long Handle, Size;
        short Padding;
        unsigned char Status, Locked;
        CHeader** RefAvailable;
        CHeader* NextAvailable;
        CHeader* PrevContiguous;
        CHeader* NextContiguous;
    };
    class CHandleInfo {
    public:
        union { CHeader* Header; long NextUnusedHandle; };
    };
    void* Buffer;
    void* UnalignedBuffer;
    long PoolSize, AllocatedSize, NumAllocations, LockCount, LargestFreeArea;
    unsigned long MemoryFlags;
    bool MultiThreaded, ShutdownFlag, EnableDefrag, EnableFullDefragIfPartialFails;
    unsigned long MaxFreeSizeBeforeDefrag, MaxFreeSizeBeforeFailure;
    long MaxBlocksToTryOnPartialDefrag;
    CFailedAllocationHandler* FailureHandler;
    CHeader* FreeLists[256];
    CArrayVector<CHandleInfo> Handles;
    long FirstUnusedHandle;
    CCharString Name;
    unsigned char CriticalSection[0x18];
protected:
    virtual void MoveResource(void*, long, void*, void*, unsigned long);
    virtual bool CanMoveResource(void*, long);
    virtual void ValidateResource(void*);
private:
    void InsertFreeArea(CHeader* header);
    void RemoveFreeArea(CHeader* header)
    {
        if (header->RefAvailable && header->Status == STATUS_FREE) {
            header->Status = STATUS_NULL;
            *header->RefAvailable = header->NextAvailable;
            if (header->NextAvailable)
                header->NextAvailable->RefAvailable = header->RefAvailable;
            header->RefAvailable = 0;
            header->NextAvailable = 0;
        }
    }
    void CombineAreas(CHeader* first, CHeader* second);
    // The free-list bucket of a size; retail keeps an out-of-line copy at
    // 0x00A01D00 (not named in Ego_r, where it is inlined everywhere).
    // FindFreeArea spells the clamp as a ternary instead (see there).
    unsigned long GetFreeListIndex(long size)
    {
        unsigned long index = (unsigned long)size / FREE_LIST_SIZE_STEP;
        if (index >= FREE_LIST_COUNT - 1)
            index = FREE_LIST_COUNT - 1;
        return index;
    }
    CHeader* CreateArea(void* buffer, unsigned long size, CHeader* prev, CHeader* next);
    CHeader* FindFreeArea(long size);
    CHeader* FindLargestFreeArea();
    void UpdateLargestFreeArea();
    void ValidateLargestFreeArea();
    bool TestIfAreaIsGrowable(CHeader* header, long size);
    void ClearFreeLists();
    bool BeginCritical(bool try_only);
    void EndCritical();
    void ResizeArea(CHeader* header, long size);
    void CombineAdjacentAreas(CHeader*& header);
    void MoveArea(CHeader* from, CHeader* to);
    bool GrowFreeArea(CHeader*& area);
    long InternalAllocateHandle();
    bool InternalAllocate(long* handle, unsigned long size);
    void InternalFreeHandle(long handle)
    {
        CHandleInfo& info = Handles[handle];
        info.Header = 0;
        info.NextUnusedHandle = FirstUnusedHandle;
        FirstUnusedHandle = handle;
    }
};
// Ego_r CScopedMemoryHandle (8 bytes): owns one pool allocation. Initialise
// and Clear are inline in retail (Allocate(CScopedMemoryHandle&) 0x00A032F0).
class CScopedMemoryHandle {
public:
    // FableWin 03363260 / 03369910: an empty owner starts invalid and
    // destruction follows the same release path as explicit Clear.
    CScopedMemoryHandle() : Pool(0), Handle(-1) {}
    ~CScopedMemoryHandle() { Clear(); }
    void Clear()
    {
        if (Pool) {
            Pool->Free(Handle);
            Pool = 0;
            Handle = -1;
        }
    }
    // Ego_r inline: the allocation's current address.
    void* GetPointer() const { return Pool->GetPointerFromHandle(Handle); }
    void Initialise(CHandleBasedMemoryPool* pool, long handle)
    {
        Clear();
        Pool = pool;
        Handle = handle;
    }
    CHandleBasedMemoryPool* Pool;
    long Handle;
};
FABLE_STATIC_ASSERT(sizeof(CScopedMemoryHandle)==8);
class CMovableResourceMemoryPool : public CHandleBasedMemoryPool {
public:
    CMovableResourceMemoryPool();
    // Implicit virtual destructor: retail slot 0 (00A62F90) calls the base
    // destructor without resetting a derived vptr first.
    void FreeResource(long handle);
    bool AllocateResource(long*,CMovableResource*,unsigned long,unsigned long);
    // Ego_r SetMovableResource; retail 00A62EB0 forwards to InternalSetResource.
    void SetMovableResource(long handle, CMovableResource* resource);
protected:
    virtual void MoveResource(void*, long, void*, void*, unsigned long);
    virtual bool CanMoveResource(void*, long);
    virtual void ValidateResource(void*);
};
// Retail 0x00A01D20 is out of line (CHandleBasedMemoryPool_Areas_00a01d20.cpp
// defines FABLE_HANDLE_POOL_HEADER_OUT_OF_LINE); fixtures that declare header
// arrays get this inline copy. The handle is left for the allocator to set.
#ifndef FABLE_HANDLE_POOL_HEADER_OUT_OF_LINE
inline CHandleBasedMemoryPool::CHeader::CHeader()
    : Resource(0), Size(0), Padding(0), Status(STATUS_NULL), Locked(0),
      RefAvailable(0), NextAvailable(0), PrevContiguous(0), NextContiguous(0)
{
}
// The scoped guard is expanded by callers outside lib_handle_based_memory_pool
// too (AllocateResource 00A62E40). Its standalone copies remain in the base TU.
inline CHandleBasedMemoryPool::CScopedCritical::CScopedCritical(CHandleBasedMemoryPool& pool, bool try_only)
    : Pool(pool)
{
    Entered = pool.BeginCritical(try_only);
}
inline CHandleBasedMemoryPool::CScopedCritical::~CScopedCritical()
{
    if (Entered)
        Pool.EndCritical();
}
#endif
FABLE_STATIC_ASSERT(sizeof(CHandleBasedMemoryPool::CHeader)==0x20);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool::CHeader,Status)==0x0e);
FABLE_STATIC_ASSERT(sizeof(CHandleBasedMemoryPool::CHandleInfo)==4);
FABLE_STATIC_ASSERT(sizeof(CHandleBasedMemoryPool)==0x464);
FABLE_STATIC_ASSERT(sizeof(CMovableResourceMemoryPool)==0x464);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool,Handles)==0x438);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool,FirstUnusedHandle)==0x444);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool,CriticalSection)==0x44c);
