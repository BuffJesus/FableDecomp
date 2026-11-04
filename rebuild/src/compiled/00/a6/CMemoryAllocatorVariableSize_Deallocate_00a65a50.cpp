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
inline CMemoryAllocatorVariableSize::CMemoryArea* __fastcall
CMemoryAllocatorVariableSize::CMemoryArea::ConvertDataPointerToMemoryArea(void* data)
{
    return reinterpret_cast<CMemoryArea*>(static_cast<unsigned char*>(data)-GetHeaderSize());
}
void CMemoryAllocatorVariableSize::Deallocate(void* data)
{
    SetCurrentAlignment();
    CMemoryArea* area=CMemoryArea::ConvertDataPointerToMemoryArea(data);
    CMemoryAreaChunk* chunk=area->PParentChunk;
    chunk->FreeArea(area);
    if(!chunk->NoAllocatedAreas && !chunk->Permanent)DeleteChunk(chunk);
}
