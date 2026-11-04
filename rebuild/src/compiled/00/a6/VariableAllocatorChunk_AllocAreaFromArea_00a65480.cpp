#include "fable_variable_size_allocator.h"

inline long __fastcall CMemoryAllocatorVariableSize::CMemoryArea::GetHeaderSize()
{
    return ((g_FableVariableAllocatorAlignment_013D28D4+sizeof(CMemoryAllocatorVariableSize::CMemoryArea)-1)>>g_FableVariableAllocatorAlignmentShift_013D28D8)<<g_FableVariableAllocatorAlignmentShift_013D28D8;
}
inline CMemoryAllocatorVariableSize::CMemoryArea* __fastcall CMemoryAllocatorVariableSize::CMemoryArea::Create(void* memory,
    CMemoryAllocatorVariableSize::CMemoryAreaChunk* parent,unsigned long size)
{
    CMemoryAllocatorVariableSize::CMemoryArea* area=static_cast<CMemoryAllocatorVariableSize::CMemoryArea*>(memory);
    area->PNextArea=0;
    area->PRightChild=0;
    area->PPreviousArea=0;
    area->PParentArea=0;
    area->PLeftChild=0;
    area->Colour=0;
    area->PParentChunk=parent;
    area->DataSize=size;
    return area;
}
inline long CMemoryAllocatorVariableSize::CMemoryArea::GetTotalSize() const
{
    return DataSize+GetHeaderSize();
}
inline void CMemoryAllocatorVariableSize::CMemoryAreaChunk::InsertAreaIntoAllocatedList(CMemoryArea* area)
{
    area->PNextArea=PFirstAllocatedArea;
    area->PPreviousArea=0;
    if(PFirstAllocatedArea)PFirstAllocatedArea->PPreviousArea=area;
    PFirstAllocatedArea=area;
    ++NoAllocatedAreas;
}

CMemoryAllocatorVariableSize::CMemoryArea*
CMemoryAllocatorVariableSize::CMemoryAreaChunk::AllocAreaFromArea(CMemoryArea* area,long size)
{
    __int64 checked=CMemoryArea::GetHeaderSize()+size;
    if(checked<0 || checked>0x7fffffff)return 0;
    long total=CMemoryArea::GetHeaderSize()+size;
    if(area->DataSize>total) {
        RemoveAreaFromFreeList(area);
        long remainder=area->GetTotalSize()-total-CMemoryArea::GetHeaderSize();
        CMemoryArea* freeArea=CMemoryArea::Create(area,this,remainder);
        CMemoryArea* allocated=CMemoryArea::Create(reinterpret_cast<unsigned char*>(area)+area->GetTotalSize(),this,size);
        InsertAreaIntoFreeList(freeArea);
        InsertAreaIntoAllocatedList(allocated);
        return allocated;
    }
    RemoveAreaFromFreeList(area);
    InsertAreaIntoAllocatedList(area);
    return area;
}
