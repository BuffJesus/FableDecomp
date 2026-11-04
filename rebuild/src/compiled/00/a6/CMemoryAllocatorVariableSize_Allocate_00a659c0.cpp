#include "fable_variable_size_allocator.h"
inline unsigned long __fastcall GFAlignShift(unsigned long size,unsigned long alignment,unsigned long shift)
{
    return ((alignment+size-1)>>shift)<<shift;
}
inline unsigned long CMemoryAllocatorBase::Align(unsigned long size) const
{
    return GFAlignShift(size,Alignment,AlignmentShift);
}
inline CMemoryAllocatorVariableSize::CMemoryArea*
CMemoryAllocatorVariableSize::AllocateMemoryFromExistingChunksInline(unsigned long size)
{
    size=Align(size);
    CMemoryArea* area=GetTreeAreaWithSizeGreaterThanOrEqualTo(size);
    if(area)return area->PParentChunk->AllocAreaFromArea(area,size);
    return 0;
}
inline long __fastcall CMemoryAllocatorVariableSize::CMemoryArea::GetHeaderSize()
{
    return ((g_FableVariableAllocatorAlignment_013D28D4+sizeof(CMemoryArea)-1)>>g_FableVariableAllocatorAlignmentShift_013D28D8)<<g_FableVariableAllocatorAlignmentShift_013D28D8;
}
inline void* CMemoryAllocatorVariableSize::CMemoryArea::GetDataStart() const
{
    return reinterpret_cast<unsigned char*>(const_cast<CMemoryArea*>(this))+GetHeaderSize();
}
inline void CMemoryAllocatorBase::SetCurrentAlignment()
{
    g_FableVariableAllocatorAlignmentShift_013D28D8=AlignmentShift;
    g_FableVariableAllocatorAlignment_013D28D4=Alignment;
}
void* CMemoryAllocatorVariableSize::Allocate(unsigned long size)
{
    SetCurrentAlignment();
    CMemoryArea* area=AllocateMemoryFromExistingChunksInline(size);
    if(!area) {
        if(NewChunk(size))area=AllocateMemoryFromExistingChunks(size);
    }
    if(area)return area->GetDataStart();
    return 0;
}
