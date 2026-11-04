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
inline void* CMemoryAllocatorVariableSize::CMemoryArea::GetDataStart() const
{
    return reinterpret_cast<unsigned char*>(const_cast<CMemoryArea*>(this))+GetHeaderSize();
}
unsigned long CMemoryAllocatorVariableSize::GetAllAllocatedAreas(void** areas,unsigned long max)
{
    SetCurrentAlignment();
    unsigned long count=0;
    for(CMemoryAreaChunk* chunk=PFirstChunk;chunk;chunk=chunk->PNextChunk)
        for(CMemoryArea* area=chunk->PFirstAllocatedArea;area;area=area->PNextArea)
            areas[count++]=area->GetDataStart();
    return count;
}
void* CMemoryAllocatorVariableSize::GetAllocatedArea(long index)
{
    SetCurrentAlignment();
    long count=0;
    for(CMemoryAreaChunk* chunk=PFirstChunk;chunk;chunk=chunk->PNextChunk)
        for(CMemoryArea* area=chunk->PFirstAllocatedArea;area;area=area->PNextArea)
        {
            if(count==index)return area->GetDataStart();
            ++count;
        }
    return 0;
}
