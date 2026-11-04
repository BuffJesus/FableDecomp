#pragma once

#include "rebuild_abi.h"

// Moved verbatim from fable_game_text_bank.h so CDefinitionManager (a
// CResourceList in retail) can derive from it without the text-bank types.

// Intrusive reference count; slot 1 destroys the pointee when it reaches 0.
// Vtable 0x012354D0. The ctor and dtor are inline in retail: every derived
// ctor stores IVRefCount = 1 (e.g. 0x00550810, 0x005DF5B0, CResourceList
// 0x009FC570), and the dtor only restores this vptr (~CResource 0x009FC550).
class CIVCountedPointeeBase
{
public:
    CIVCountedPointeeBase()
        : IVRefCount(1)
    {
    }
    virtual ~CIVCountedPointeeBase()
    {
    }
    virtual void DeleteCIVCountedPointeeBase() const;

    void AddRef() const
    {
        ++IVRefCount;
    }

    long Release() const
    {
        long count = --IVRefCount;
        if (count == 0)
            DeleteCIVCountedPointeeBase();
        return count;
    }

    mutable unsigned long IVRefCount;
};

class CResourceList;

// An LRU-listed resource; CResourceList::Head is the list's sentinel.
// Vtable 0x012354A4. Retail's out-of-line ~CResource is 0x009FC550 (folded
// with ~CMemoryBlock); ResourceList.cpp inlines it into ~CResourceList
// 0x009FC5C0, so it is modelled inline here.
class CResource : public CIVCountedPointeeBase
{
public:
    // FableWin ??0CResource@@QAE@XZ, retail 0x009FBEC0: out of line (def
    // allocators such as CFrontEndDef::Alloc 0x00433080 call it). Retail's
    // ResourceList.cpp defines it and /O2's auto-inlining (/Ob2) expands it
    // into CResourceList 0x009FC570, so that landing defines it too.
    CResource();
    // FableWin 0x00431F60: copies all six non-vtable resource fields.
    CResource& operator=(const CResource& other);
    // FableWin ??1CResource@@UAE@XZ.
#ifdef FABLE_RESOURCE_DESTRUCTOR_DECLARATION_ONLY
    // Some original translation units call the standalone 0x009fc550 body.
    virtual ~CResource();
#else
    virtual ~CResource()
    {
        RemoveFromResourceList();
    }
#endif

    // Retail vtable 012354a4, slots 2..9; donor method order agrees.
    virtual bool UnloadResource() { return false; }
    virtual unsigned long GetUnloadableMemory() const { return ResourceSize; }
    virtual bool IsResourceLoaded() const { return false; }
    virtual void OnResourceLoaded() {}
    virtual bool CanAbortResourceLoad(long) const { return false; }
    virtual void OnResourceLoadAborted() {}
    virtual unsigned long GetDebugMemoryInUse() const { return ResourceSize; }
    virtual void ResetDebugMemoryInUse() {}

    // FableWin ?RemoveFromResourceList@CResource@@QAEXXZ; inline in retail.
    void RemoveFromResourceList();

    void SetAsMRU();
    void TouchResource();
    unsigned long GetUnusedFrames() const;
    void SetAsLRU();
    // FableWin ?SetResourceSize@CResource@@QAEXK@Z, retail 0x009FBF20.
    void SetResourceSize(unsigned long size);
    // Ego_r/FableWin ?SetAsUnloadable@CResource@@QAEXXZ, retail 0x009FBF50.
    void SetAsUnloadable();

    CResourceList* ResourceList;
    CResource* PrevResource;
    CResource* NextResource;
    unsigned long ResourceSize;
    unsigned long LastUsedFrame;

private:
    friend class CResourceTestAccess;
    friend class CResourceList;
    void ListRemove();
    void ListInsert(CResource* prev, CResource* next);
};

#include "fable_failed_allocation_handler.h"

// Retail sizeof 0x3C (Ego_r agrees). Vtable 0x0129C808: slot 0 scalar
// deleting destructor 0x009FC700, slot 1 HandleFailedAllocation 0x009FC540
// (inherited unchanged by CDefinitionManager, vtable 0x0129B28C).
class CResourceList : public CFailedAllocationHandler
{
public:
    // FableWin ??0CResourceList@@QAE@XZ, retail 0x009FC570 (out of line;
    // called first by CDefinitionManager 0x009B0470).
    CResourceList();
    virtual ~CResourceList();
    // FableWin ?HandleFailedAllocation@CResourceList@@UAE_NK@Z, retail 0x009FC540.
    virtual bool HandleFailedAllocation(unsigned long size);
    // FableWin ?SetResourceUnloadDelay@CResourceList@@QAEXK@Z; inline in
    // retail (CDefinitionManager 0x009B0470 ends with `mov [esi+0x34],0`).
    void SetResourceUnloadDelay(unsigned long unloadDelay)
    {
        UnloadDelay = unloadDelay;
    }
    // Ego_r/FableWin ?SetMaximumMemory@CResourceList@@QAEXK@Z, retail 0x009FC520.
    void SetMaximumMemory(unsigned long maximumMemory);
    bool MakeFreeSpace(unsigned long size);
    // FableWin ?ShrinkResources@CResourceList@@QAEXK@Z, retail 0x009FC210.
    void ShrinkResources(unsigned long size);
    // FableWin ?FrameEnd@CResourceList@@QAEXXZ, retail 0x009FC150.
    void FrameEnd();
    void ProcessLoadingQueue(bool,double);
    void IncrementFrameCounter(unsigned long frames);
    // FableWin ?AddResource@CResourceList@@QAEXAAVCResource@@@Z, retail 0x009FBFE0.
    inline void AddResource(CResource& resource);
    // FableWin ?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z, retail 0x009FC020.
    void RemoveResource(CResource& resource);
    // FableWin ?UnloadNextResource@CResourceList@@QAE_NK@Z, retail 0x009FC170.
    bool UnloadNextResource(unsigned long size);
    // Ego_r debug statistics (ResourceList.cpp; retail 0x009FC0B0 / 0x009FC100 /
    // 0x009FC140 / 0x009FC360).
    unsigned long GetDebugMemoryInUse(unsigned long frames) const;
    void ResetDebugStats();
    unsigned long GetFramesSinceResetDebugStats() const;
    void Validate();

    CResource Head;
    unsigned long ResourceCount;
    long AllocatedMemory;
    long MaximumMemory;
    unsigned long CurrentFrame;
    unsigned long DebugStatsFrame;
    unsigned long UnloadDelay;
    unsigned long UnloadedThisFrame;
};

// Original inline TouchResource: move once per frame and update LastUsedFrame.
#ifdef FABLE_RESOURCE_FORCE_INLINE_TOUCH
// Native nested frame loader expands both touches; other TUs keep their context.
__forceinline void CResource::TouchResource()
#else
inline void CResource::TouchResource()
#endif
{
    if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame)
    {
        ListRemove();
        ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
        LastUsedFrame = ResourceList->CurrentFrame;
    }
}

#ifdef FABLE_RESOURCE_NOINLINE_REMOVE
// Native9FDE80 calls the separate16B wrapper at9FC440, not the list child.
__declspec(noinline) inline void CResource::RemoveFromResourceList()
#else
inline void CResource::RemoveFromResourceList()
#endif
{
    if (ResourceList)
        ResourceList->RemoveResource(*this);
}

FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1C);
FABLE_STATIC_ASSERT(sizeof(CResourceList) == 0x3C);
