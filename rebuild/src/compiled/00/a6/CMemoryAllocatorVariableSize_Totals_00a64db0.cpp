#include "fable_variable_size_allocator.h"
inline void CMemoryAllocatorBase::SetCurrentAlignment()
{
    g_FableVariableAllocatorAlignmentShift_013D28D8=AlignmentShift;
    g_FableVariableAllocatorAlignment_013D28D4=Alignment;
}
inline long __fastcall CMemoryAllocatorVariableSize::CMemoryArea::GetHeaderSize()
{
    return ((g_FableVariableAllocatorAlignment_013D28D4+sizeof(CMemoryArea)-1)>>g_FableVariableAllocatorAlignmentShift_013D28D8)<<g_FableVariableAllocatorAlignmentShift_013D28D8;
}
inline unsigned long __fastcall CMemoryAllocatorVariableSize::CMemoryAreaChunk::GetHeaderSize()
{
    return ((g_FableVariableAllocatorAlignment_013D28D4+sizeof(CMemoryAreaChunk)-1)>>g_FableVariableAllocatorAlignmentShift_013D28D8)<<g_FableVariableAllocatorAlignmentShift_013D28D8;
}
inline long CMemoryAllocatorVariableSize::CMemoryAreaChunk::CalcAllocatedDataSize() const
{
    long data=0;
    for(CMemoryArea* area=PFirstAllocatedArea;area;area=area->PNextArea)data+=area->DataSize;
    return data;
}
inline long CMemoryAllocatorVariableSize::CMemoryAreaChunk::GetAllocatedDataSize() const
{
    return CalcAllocatedDataSize();
}
unsigned long CMemoryAllocatorVariableSize::GetTotalMemoryAllocated()
{
    SetCurrentAlignment();
    unsigned long total=0;
    for(CMemoryAreaChunk* chunk=PFirstChunk;chunk;chunk=chunk->PNextChunk){
        total+=chunk->GetAllocatedDataSize()+chunk->NoAllocatedAreas*CMemoryArea::GetHeaderSize()+CMemoryAreaChunk::GetHeaderSize();
    }
    return total;
}
unsigned long CMemoryAllocatorVariableSize::GetHeaderOverhead()
{
    SetCurrentAlignment();
    unsigned long total=0;
    for(CMemoryAreaChunk* chunk=PFirstChunk;chunk;chunk=chunk->PNextChunk)
        total+=chunk->NoAllocatedAreas*CMemoryArea::GetHeaderSize()+CMemoryAreaChunk::GetHeaderSize();
    return total;
}
unsigned long CMemoryAllocatorVariableSize::GetTotalMemoryControlledByAllocator()
{
    SetCurrentAlignment();
    unsigned long total=0;
    for(CMemoryAreaChunk* chunk=PFirstChunk;chunk;chunk=chunk->PNextChunk)
        total+=chunk->TotalDataSize+CMemoryAreaChunk::GetHeaderSize()+CMemoryArea::GetHeaderSize();
    return total;
}
