#include "fable_variable_size_allocator.h"
void CMemoryAllocatorVariableSize::CMemoryAreaChunk::RemoveAreaFromFreeList(CMemoryArea* area)
{
    if(area->PPreviousArea)area->PPreviousArea->PNextArea=area->PNextArea;
    else PFirstFreeArea=area->PNextArea;
    if(area->PNextArea)area->PNextArea->PPreviousArea=area->PPreviousArea;
    else PLastFreeArea=area->PPreviousArea;
    area->PNextArea=0;
    area->PPreviousArea=0;
    PParentAllocator->RemoveFromTree(area);
    --NoFreeAreas;
    FreeDataSize-=area->DataSize;
    if(PLastFreeArea)LargestFreeAreaSize=PLastFreeArea->DataSize;
    else LargestFreeAreaSize=0;
    if(PFirstFreeArea)SmallestFreeAreaSize=PFirstFreeArea->DataSize;
    else SmallestFreeAreaSize=0;
}
