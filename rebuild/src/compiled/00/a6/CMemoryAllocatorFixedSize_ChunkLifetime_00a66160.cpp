#include "candidates/fixed_size_allocator_accessors_compiled.h"
extern "C" __declspec(dllimport) void* __stdcall GetProcessHeap();
extern "C" __declspec(dllimport) int __stdcall HeapFree(void*,unsigned long,void*);
inline void CMemoryAllocatorFixedSize::RemoveChunkFromListInline(CMemoryAreaChunk* chunk)
{
    if(chunk->PNextChunk)chunk->PNextChunk->PPreviousChunk=chunk->PPreviousChunk;
    if(chunk->PPreviousChunk)chunk->PPreviousChunk->PNextChunk=chunk->PNextChunk;
    if(PFirstChunk==chunk)PFirstChunk=chunk->PNextChunk;
    if(PLastChunk==chunk)PLastChunk=chunk->PPreviousChunk;
}
inline void CMemoryAllocatorFixedSize::InternalDealloc(void* memory)
{
    HeapFree(GetProcessHeap(),1,memory);
}
void CMemoryAllocatorFixedSize::DeleteChunk(CMemoryAreaChunk* chunk)
{
    if(PCurrentAllocatorChunk==chunk)PCurrentAllocatorChunk=0;
    RemoveChunkFromListInline(chunk);
    InternalDealloc(chunk->ThisChunkMem);
}
inline CMemoryAllocatorFixedSize::CMemoryArea* FABLE_FASTCALL CMemoryAllocatorFixedSize::CMemoryArea::Create(void* memory,unsigned char id)
{
    CMemoryArea* area=static_cast<CMemoryArea*>(memory);
    area->NextAreaID=255;area->PreviousAreaID=255;area->AreaID=id;area->AllocatorID=0;
    return area;
}
inline unsigned long CMemoryAllocatorBase::Align(unsigned long size) const
{
    return ((size+Alignment-1)>>AlignmentShift)<<AlignmentShift;
}
CMemoryAllocatorFixedSize::CMemoryAreaChunk* CMemoryAllocatorFixedSize::CreateChunk(void* memory)
{
    CMemoryAreaChunk* chunk=reinterpret_cast<CMemoryAreaChunk*>(Align(reinterpret_cast<unsigned long>(memory)));
    chunk->ThisChunkMem=memory;chunk->PNextChunk=0;chunk->PPreviousChunk=0;
    chunk->PFirstFreeArea=0;chunk->PFirstAllocatedArea=0;chunk->NoAllocatedAreas=0;
    unsigned char* next=reinterpret_cast<unsigned char*>(chunk)+ChunkHeaderSize;
    for(unsigned long i=0;i<NoAreasPerChunk;++i){
        CMemoryArea* area=CMemoryArea::Create(next,static_cast<unsigned char>(i));
        area->NextAreaID=chunk->PFirstFreeArea?chunk->PFirstFreeArea->AreaID:255;
        if(chunk->PFirstFreeArea)chunk->PFirstFreeArea->PreviousAreaID=static_cast<unsigned char>(i);
        chunk->PFirstFreeArea=area;next+=StatAreaSize;
    }
    return chunk;
}
