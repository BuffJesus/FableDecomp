#include "fable_variable_size_allocator.h"

void CMemoryAllocatorVariableSize::CMemoryAreaChunk::Init(
    void* memory,long size,CMemoryAllocatorVariableSize* allocator)
{
    PParentAllocator=allocator;
    PNextChunk=0;
    PPreviousChunk=0;
    PFirstFreeArea=0;
    PLastFreeArea=0;
    PFirstAllocatedArea=0;
    NoFreeAreas=0;
    NoAllocatedAreas=0;
    LargestFreeAreaSize=0;
    SmallestFreeAreaSize=0;
    long dataSize=size-(((g_FableVariableAllocatorAlignment_013D28D4+sizeof(CMemoryArea)-1)>>
        g_FableVariableAllocatorAlignmentShift_013D28D8)<<g_FableVariableAllocatorAlignmentShift_013D28D8);
    CMemoryArea* area=static_cast<CMemoryArea*>(memory);
    area->PNextArea=0;
    area->PRightChild=0;
    area->PPreviousArea=0;
    area->PParentArea=0;
    area->PLeftChild=0;
    area->Colour=0;
    area->PParentChunk=this;
    area->DataSize=dataSize;
    TotalDataSize=dataSize;
    FreeDataSize=0;
    InsertAreaIntoFreeList(area);
}
