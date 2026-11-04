#include "candidates/fixed_size_allocator_accessors_compiled.h"
inline CMemoryAllocatorFixedSize::CMemoryArea* CMemoryAllocatorFixedSize::GetPNextArea(CMemoryArea* area) const
{
    if(area->NextAreaID==255)return 0;
    return reinterpret_cast<CMemoryArea*>(reinterpret_cast<unsigned char*>(area)+(static_cast<long>(area->NextAreaID)-area->AreaID)*StatAreaSize);
}
unsigned long CMemoryAllocatorFixedSize::GetAllAllocatedAreas(void** areas,unsigned long max)
{
    unsigned long count=0;
    GetNoAllocatedAreas();
    for(CMemoryAreaChunk* chunk=PFirstChunk;chunk;chunk=chunk->PNextChunk)
        for(CMemoryArea* area=chunk->PFirstAllocatedArea;area;area=GetPNextArea(area))
            areas[count++]=reinterpret_cast<unsigned char*>(area)+sizeof(CMemoryArea);
    return count;
}
void* CMemoryAllocatorFixedSize::GetAllocatedArea(long index)
{
    long count=0;
    for(CMemoryAreaChunk* chunk=PFirstChunk;chunk;chunk=chunk->PNextChunk)
        for(CMemoryArea* area=chunk->PFirstAllocatedArea;area;area=GetPNextArea(area))
        {
            if(count==index)return reinterpret_cast<unsigned char*>(area)+sizeof(CMemoryArea);
            ++count;
        }
    return 0;
}
