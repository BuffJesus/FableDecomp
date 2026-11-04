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

inline void CMemoryAllocatorVariableSize::CMemoryAreaChunk::RemoveAreaFromAllocatedList(CMemoryArea* area)
{
    if(area->PPreviousArea)area->PPreviousArea->PNextArea=area->PNextArea;
    else PFirstAllocatedArea=area->PNextArea;
    if(area->PNextArea)area->PNextArea->PPreviousArea=area->PPreviousArea;
    area->PNextArea=0;
    area->PPreviousArea=0;
    --NoAllocatedAreas;
}
void CMemoryAllocatorVariableSize::CMemoryAreaChunk::ConsolidateAdjacentFreeAreas(CMemoryArea* area)
{
    CMemoryArea* active=area;
    CMemoryArea* current=PFirstFreeArea;
    while(current) {
        CMemoryArea* next=current->PNextArea;
        if(reinterpret_cast<unsigned char*>(active)+active->GetTotalSize()==reinterpret_cast<unsigned char*>(current)) {
            RemoveAreaFromFreeList(active);
            RemoveAreaFromFreeList(current);
            CMemoryArea* merged=CMemoryArea::Create(active,this,active->DataSize+current->GetTotalSize());
            InsertAreaIntoFreeList(merged);
            next=PFirstFreeArea;
            active=merged;
        } else if(reinterpret_cast<unsigned char*>(current)+current->GetTotalSize()==reinterpret_cast<unsigned char*>(active)) {
            RemoveAreaFromFreeList(active);
            RemoveAreaFromFreeList(current);
            CMemoryArea* merged=CMemoryArea::Create(current,this,current->DataSize+active->GetTotalSize());
            InsertAreaIntoFreeList(merged);
            next=PFirstFreeArea;
            active=merged;
        }
        current=next;
    }
}
void CMemoryAllocatorVariableSize::CMemoryAreaChunk::FreeArea(CMemoryArea* area)
{
    RemoveAreaFromAllocatedList(area);
    InsertAreaIntoFreeList(area);
    ConsolidateAdjacentFreeAreas(area);
}
