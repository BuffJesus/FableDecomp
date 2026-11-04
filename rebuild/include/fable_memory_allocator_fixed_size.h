#pragma once
// Hand-owned (retail model): CMemoryAllocatorFixedSize, the chunked fixed-area allocator (Ego_r names and
// complete layout). Moved verbatim from candidates/fixed_size_allocator_accessors_compiled.h, which now only
// forwards here, so gen_class_headers treats the class as hand-owned; engine/CMemoryAllocatorFixedSize.h is a shim.

#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_memory_allocator.h"

class CMemoryAllocatorFixedSize : public CMemoryAllocatorBase
{
public:
    class CMemoryArea
    {
    public:
        unsigned char AreaID,NextAreaID,PreviousAreaID,AllocatorID;
        static CMemoryArea* FABLE_FASTCALL Create(void* memory,unsigned char id);
    };
    class CMemoryAreaChunk
    {
    public:
        void* ThisChunkMem;
        long NoAllocatedAreas;
        CMemoryAreaChunk* PNextChunk;
        CMemoryAreaChunk* PPreviousChunk;
        CMemoryArea* PFirstFreeArea;
        CMemoryArea* PFirstAllocatedArea;
    };
    // Names and complete layout from Ego_r's CMemoryAllocatorFixedSize.
    CMemoryAreaChunk* PFirstChunk;
    CMemoryAreaChunk* PLastChunk;
    CMemoryAreaChunk* PCurrentAllocatorChunk;
    unsigned long AreaSize;
    unsigned long StatAreaSize;
    unsigned long ChunkHeaderSize;
    unsigned long NoAreasPerChunk;
    unsigned long ChunkSize;
    unsigned long Flags;
    unsigned long InternalFlags;
    unsigned long ExtraMemUsedByOS;
    long NoEmptyChunks;

    CMemoryAllocatorFixedSize(void* heap, unsigned long minimumChunkSize,
        unsigned long areaSize, unsigned long alignment, unsigned long flags,
        unsigned long internalFlags);
    virtual void* Allocate(unsigned long size);
    virtual void Deallocate(void* area);
    virtual unsigned long GetAllAllocatedAreas(void** areas, unsigned long max);
    virtual unsigned long GetAllocationSize(void* allocation);
    virtual unsigned long GetAllocationSizeIncludingHeader(void* allocation);
    virtual unsigned long GetLargestFreeAreaSize();
    virtual unsigned long GetTotalMemoryControlledByAllocator();
    virtual unsigned long GetTotalMemoryAllocated();
    virtual unsigned long GetNoFreeAreas();
    virtual unsigned long GetNoAllocatedAreas();
    virtual float GetFragmentation();
    virtual void* GetAllocatedArea(long index);
    virtual unsigned long GetHeaderOverhead();
    virtual void ConsolidateFreeAreas();
    virtual void Validate();
    virtual ~CMemoryAllocatorFixedSize(); // added slot 15; base has no virtual dtor
protected:
    CMemoryArea* GetPNextArea(CMemoryArea* area) const;
    CMemoryArea* GetPPreviousArea(CMemoryArea* area) const;
    CMemoryAreaChunk* GetPParentChunk(CMemoryArea* area) const;
    void Validate(CMemoryAreaChunk* chunk);
    void RemoveChunkFromList(CMemoryAreaChunk* chunk);
    void InsertChunkAtListHead(CMemoryAreaChunk* chunk);
    void InsertChunkAtListTail(CMemoryAreaChunk* chunk);
    void DeleteChunk(CMemoryAreaChunk* chunk);
    CMemoryAreaChunk* CreateChunk(void* memory);
    void InternalDealloc(void* memory);
    // Implementation helper for the retail DeleteChunk body's inlined removal.
    void RemoveChunkFromListInline(CMemoryAreaChunk* chunk);
    unsigned long InternalAlloc(unsigned long size,void** memory);
    CMemoryAreaChunk* NewChunk();
    void InsertChunkAtListHeadInline(CMemoryAreaChunk* chunk);
};

typedef CMemoryAllocatorFixedSize::CMemoryAreaChunk CFixedSizeAllocatorNode;
FABLE_STATIC_ASSERT(sizeof(CMemoryAllocatorFixedSize::CMemoryArea)==4);
FABLE_STATIC_ASSERT(sizeof(CFixedSizeAllocatorNode) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CFixedSizeAllocatorNode, NoAllocatedAreas) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CFixedSizeAllocatorNode, PNextChunk) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorFixedSize, PFirstChunk) == 0xC1C);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorFixedSize, AreaSize) == 0xC28);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorFixedSize, StatAreaSize) == 0xC2C);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorFixedSize, ChunkHeaderSize) == 0xC30);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorFixedSize, NoAreasPerChunk) == 0xC34);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorFixedSize, ChunkSize) == 0xC38);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorFixedSize, ExtraMemUsedByOS) == 0xC44);
FABLE_STATIC_ASSERT(offsetof(CMemoryAllocatorFixedSize, NoEmptyChunks) == 0xC48);
FABLE_STATIC_ASSERT(sizeof(CMemoryAllocatorFixedSize) == 0xC4C);
