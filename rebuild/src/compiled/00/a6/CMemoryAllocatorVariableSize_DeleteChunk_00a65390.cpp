#include "fable_variable_size_allocator.h"
struct FableMemoryStatus {
    unsigned long Length,MemoryLoad,TotalPhysical,AvailablePhysical;
    unsigned long TotalPageFile,AvailablePageFile,TotalVirtual,AvailableVirtual;
};
extern "C" __declspec(dllimport) void __stdcall GlobalMemoryStatus(FableMemoryStatus*);
void CMemoryAllocatorVariableSize::DeleteChunk(CMemoryAreaChunk* chunk)
{
    CMemoryArea* area=chunk->PFirstFreeArea;
    while(area) {
        RemoveFromTree(area);
        area=area->PNextArea;
    }
    if(chunk->PNextChunk)chunk->PNextChunk->PPreviousChunk=chunk->PPreviousChunk;
    if(chunk->PPreviousChunk)chunk->PPreviousChunk->PNextChunk=chunk->PNextChunk;
    if(PFirstChunk==chunk)PFirstChunk=chunk->PNextChunk;
    FableMemoryStatus before;
    GlobalMemoryStatus(&before);
    if(!chunk->DemandPaged)VirtualFree(chunk,0,0x8000);
    GlobalMemoryStatus(&before);
}
