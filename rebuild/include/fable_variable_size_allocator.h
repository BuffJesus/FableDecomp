#pragma once
#include <stddef.h>
#include "fable_memory_allocator.h"

extern unsigned long g_FableVariableAllocatorAlignment_013D28D4;
extern unsigned long g_FableVariableAllocatorAlignmentShift_013D28D8;
class CDeletedNodeChildInfo;

class CMemoryAllocatorVariableSize : public CMemoryAllocatorBase
{
public:
    class CMemoryAreaChunk;
    class CMemoryArea {
    public:
        static long __fastcall GetHeaderSize();
        static CMemoryArea* __fastcall Create(void* memory,CMemoryAreaChunk* parent,unsigned long size);
        long GetTotalSize() const;
        void* GetDataStart() const;
        static CMemoryArea* __fastcall ConvertDataPointerToMemoryArea(void* data);
        CMemoryArea* PNextArea;
        CMemoryArea* PRightChild;
        CMemoryArea* PPreviousArea;
        CMemoryArea* PParentArea;
        CMemoryArea* PLeftChild;
        CMemoryAreaChunk* PParentChunk;
        long DataSize;
        unsigned char Colour, ID;
    };
    class CMemoryAreaChunk {
    public:
        static unsigned long __fastcall GetHeaderSize();
        long GetAllocatedDataSize() const;
        void Init(void* memory,long size,CMemoryAllocatorVariableSize* allocator);
        CMemoryArea* AllocAreaFromArea(CMemoryArea* area,long size);
        void FreeArea(CMemoryArea* area);
        void ConsolidateAdjacentFreeAreas(CMemoryArea* area);
        long TotalDataSize, NoFreeAreas, NoAllocatedAreas, FreeDataSize;
        long LargestFreeAreaSize, SmallestFreeAreaSize;
        CMemoryAllocatorVariableSize* PParentAllocator;
        bool Permanent, DemandPaged;
        CMemoryAreaChunk* PNextChunk;
        CMemoryAreaChunk* PPreviousChunk;
        CMemoryArea* PFirstFreeArea;
        CMemoryArea* PLastFreeArea;
        CMemoryArea* PFirstAllocatedArea;
    protected:
        long CalcAllocatedDataSize() const;
        void InsertAreaIntoFreeList(CMemoryArea* area);
        void RemoveAreaFromFreeList(CMemoryArea* area);
        void InsertAreaIntoAllocatedList(CMemoryArea* area);
        void RemoveAreaFromAllocatedList(CMemoryArea* area);
    };
    CMemoryAllocatorVariableSize(void* heap,long minimumChunkSize,
        void* initialMemory,long initialSize,unsigned long alignment,
        unsigned long flags,unsigned long internalFlags);
    long InternalAlloc(long size,void** memory);
    CMemoryArea* AllocateMemoryFromExistingChunks(unsigned long size);
    virtual void* Allocate(unsigned long size);
    virtual void Deallocate(void* area);
    virtual unsigned long GetAllAllocatedAreas(void** areas,unsigned long max);
    virtual unsigned long GetAllocationSize(void* area);
    virtual unsigned long GetAllocationSizeIncludingHeader(void* area);
    virtual unsigned long GetLargestFreeAreaSize();
    virtual unsigned long GetTotalMemoryControlledByAllocator();
    virtual unsigned long GetTotalMemoryAllocated();
    virtual unsigned long GetNoFreeAreas();
    virtual unsigned long GetNoAllocatedAreas();
    virtual float GetFragmentation();
    virtual void* GetAllocatedArea(long index);
    virtual unsigned long GetHeaderOverhead();
    // Slot 13 is inherited CMemoryAllocatorBase::ConsolidateFreeAreas.
    virtual void Validate();
    virtual ~CMemoryAllocatorVariableSize();

    CMemoryAreaChunk* PFirstChunk;
    long MinChunkSize;
    unsigned long NoFreesSinceConsolidation;
    unsigned long Flags, InternalFlags, ExtraMemUsedByOS;
    CMemoryArea* PTreeRoot;
protected:
    // Implementation helper for the retail virtual entry's inlined first attempt.
    CMemoryArea* AllocateMemoryFromExistingChunksInline(unsigned long size);
    void SetTreeRootNode(CMemoryArea* area);
    CMemoryArea* GetTreeRoot() const;
    void InsertIntoTree(CMemoryArea* root,CMemoryArea* area);
    void RemoveFromTree(CMemoryArea* area);
    void RemoveFromTreeFixup(CMemoryArea* area,const CDeletedNodeChildInfo* info);
    bool IsNodeBlack(const CMemoryArea* area) const;
    void InsertFixup(CMemoryArea* area);
    void RotateLeft(CMemoryArea* area);
    void RotateRight(CMemoryArea* area);
    CMemoryArea* GetTreeAreaWithSizeGreaterThanOrEqualTo(long size) const;
    long GetLargestTreeAreaSize() const;
    long GetSmallestTreeAreaSize() const;
    CMemoryAreaChunk* NewChunk(long size);
    void DeleteChunk(CMemoryAreaChunk* chunk);
    CMemoryAreaChunk* NewChunkDemandPaged(void* memory,long size);
};
class CDeletedNodeChildInfo {
public:
    bool LeftChild;
    CMemoryAllocatorVariableSize::CMemoryArea* PParent;
    unsigned char Colour;
    bool Valid;
};
FABLE_STATIC_ASSERT(sizeof(CDeletedNodeChildInfo)==0xc);
FABLE_STATIC_ASSERT(sizeof(CMemoryAllocatorVariableSize)==0xc38);
FABLE_STATIC_ASSERT(sizeof(CMemoryAllocatorVariableSize::CMemoryArea)==0x20);
FABLE_STATIC_ASSERT(sizeof(CMemoryAllocatorVariableSize::CMemoryAreaChunk)==0x34);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorVariableSize,PFirstChunk)==0xc1c);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorVariableSize,PTreeRoot)==0xc34);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorVariableSize::CMemoryAreaChunk,Permanent)==0x1c);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorVariableSize::CMemoryAreaChunk,PNextChunk)==0x20);
