#include "candidates/fixed_size_allocator_accessors_compiled.h"
inline CMemoryAllocatorFixedSize::CMemoryArea* CMemoryAllocatorFixedSize::GetPNextArea(CMemoryArea* area) const
{
    if(area->NextAreaID==255)return 0;
    return reinterpret_cast<CMemoryArea*>(reinterpret_cast<unsigned char*>(area)+(static_cast<long>(area->NextAreaID)-area->AreaID)*StatAreaSize);
}
void CMemoryAllocatorFixedSize::Validate(CMemoryAreaChunk* chunk)
{
    for(CMemoryArea* area=chunk->PFirstAllocatedArea;area;area=GetPNextArea(area))
        CMemoryAllocator::Get()->InternalValidateArea(reinterpret_cast<unsigned char*>(area)+sizeof(CMemoryArea));
}
void CMemoryAllocatorFixedSize::Validate()
{
    for(CMemoryAreaChunk* chunk=PFirstChunk;chunk;chunk=chunk->PNextChunk)Validate(chunk);
}
