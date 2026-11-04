#include "fable_variable_size_allocator.h"

void CMemoryAllocatorVariableSize::CMemoryAreaChunk::InsertAreaIntoFreeList(CMemoryArea* area)
{
    CMemoryArea* current=PFirstFreeArea;
    if(!current) {
        area->PNextArea=0;
        area->PPreviousArea=0;
        PFirstFreeArea=area;
        PLastFreeArea=area;
    } else {
        while(current) {
            if(current->DataSize>area->DataSize) {
                area->PNextArea=current;
                area->PPreviousArea=current->PPreviousArea;
                if(current->PPreviousArea)current->PPreviousArea->PNextArea=area;
                else PFirstFreeArea=area;
                current->PPreviousArea=area;
                break;
            }
            if(!current->PNextArea) {
                area->PNextArea=0;
                area->PPreviousArea=current;
                current->PNextArea=area;
                PLastFreeArea=area;
                break;
            }
            current=current->PNextArea;
        }
    }
    PParentAllocator->InsertIntoTree(PParentAllocator->PTreeRoot,area);
    FreeDataSize+=area->DataSize;
    if(PLastFreeArea)LargestFreeAreaSize=PLastFreeArea->DataSize;
    else LargestFreeAreaSize=0;
    if(PFirstFreeArea)SmallestFreeAreaSize=PFirstFreeArea->DataSize;
    else SmallestFreeAreaSize=0;
    ++NoFreeAreas;
}
