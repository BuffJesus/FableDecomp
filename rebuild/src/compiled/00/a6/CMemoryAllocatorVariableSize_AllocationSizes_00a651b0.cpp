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
inline CMemoryAllocatorVariableSize::CMemoryArea* __fastcall CMemoryAllocatorVariableSize::CMemoryArea::ConvertDataPointerToMemoryArea(void* data)
{
    return reinterpret_cast<CMemoryArea*>(reinterpret_cast<unsigned char*>(data)-GetHeaderSize());
}
inline long CMemoryAllocatorVariableSize::CMemoryArea::GetTotalSize() const
{
    return DataSize+GetHeaderSize();
}
unsigned long CMemoryAllocatorVariableSize::GetAllocationSize(void* data)
{
    SetCurrentAlignment();
    return CMemoryArea::ConvertDataPointerToMemoryArea(data)->DataSize;
}
unsigned long CMemoryAllocatorVariableSize::GetAllocationSizeIncludingHeader(void* data)
{
    SetCurrentAlignment();
    return CMemoryArea::ConvertDataPointerToMemoryArea(data)->GetTotalSize();
}
