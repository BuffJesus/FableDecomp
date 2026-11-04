#include "candidates/fixed_size_allocator_accessors_compiled.h"
inline CMemoryAllocatorFixedSize::CMemoryArea* CMemoryAllocatorFixedSize::GetPNextArea(CMemoryArea* area) const
{
    if(area->NextAreaID!=255)return reinterpret_cast<CMemoryArea*>(reinterpret_cast<unsigned char*>(area)+(static_cast<long>(area->NextAreaID)-area->AreaID)*StatAreaSize);
    return 0;
}
inline CMemoryAllocatorFixedSize::CMemoryArea* CMemoryAllocatorFixedSize::GetPPreviousArea(CMemoryArea* area) const
{
    if(area->PreviousAreaID==255)return 0;
    return reinterpret_cast<CMemoryArea*>(reinterpret_cast<unsigned char*>(area)+(static_cast<long>(area->PreviousAreaID)-area->AreaID)*StatAreaSize);
}
inline CMemoryAllocatorFixedSize::CMemoryAreaChunk* CMemoryAllocatorFixedSize::GetPParentChunk(CMemoryArea* area) const
{
    if(!area)return 0;
    return reinterpret_cast<CMemoryAreaChunk*>(reinterpret_cast<unsigned char*>(area)-area->AreaID*StatAreaSize-ChunkHeaderSize);
}
void* CMemoryAllocatorFixedSize::Allocate(unsigned long size)
{
    if(!PCurrentAllocatorChunk||NoAreasPerChunk-PCurrentAllocatorChunk->NoAllocatedAreas==0){
        PCurrentAllocatorChunk=PFirstChunk;
        if(!PCurrentAllocatorChunk||NoAreasPerChunk-PCurrentAllocatorChunk->NoAllocatedAreas==0){
            NewChunk();PCurrentAllocatorChunk=PFirstChunk;
        }
    }
    CMemoryAreaChunk* chunk=PCurrentAllocatorChunk;
    CMemoryArea* area=chunk->PFirstFreeArea;
    long previouslyAllocated=chunk->NoAllocatedAreas;
    if(area){
    chunk->PFirstFreeArea=GetPNextArea(area);
    if(chunk->PFirstAllocatedArea){
        area->NextAreaID=chunk->PFirstAllocatedArea->AreaID;
        chunk->PFirstAllocatedArea->PreviousAreaID=area->AreaID;
    }else area->NextAreaID=255;
    chunk->PFirstAllocatedArea=area;++chunk->NoAllocatedAreas;area->PreviousAreaID=255;
    if(!previouslyAllocated)--NoEmptyChunks;
    if(NoAreasPerChunk-PCurrentAllocatorChunk->NoAllocatedAreas==0){
        RemoveChunkFromList(PCurrentAllocatorChunk);InsertChunkAtListTail(PCurrentAllocatorChunk);
    }
    PCurrentAllocatorChunk=PFirstChunk;
    return reinterpret_cast<unsigned char*>(area)+sizeof(CMemoryArea);
    }
    return 0;
}
void CMemoryAllocatorFixedSize::Deallocate(void* memory)
{
    CMemoryArea* area=reinterpret_cast<CMemoryArea*>(static_cast<unsigned char*>(memory)-sizeof(CMemoryArea));
    CMemoryAreaChunk* chunk=GetPParentChunk(area);
    if(area->PreviousAreaID!=255)GetPPreviousArea(area)->NextAreaID=area->NextAreaID;
    else chunk->PFirstAllocatedArea=GetPNextArea(area);
    if(area->NextAreaID!=255)GetPNextArea(area)->PreviousAreaID=area->PreviousAreaID;
    if(chunk->PFirstFreeArea){
        chunk->PFirstFreeArea->PreviousAreaID=area->AreaID;
        area->NextAreaID=chunk->PFirstFreeArea->AreaID;
    }else area->NextAreaID=255;
    chunk->PFirstFreeArea=area;--chunk->NoAllocatedAreas;
    if(NoAreasPerChunk-chunk->NoAllocatedAreas==1&&PFirstChunk!=chunk){
        RemoveChunkFromList(chunk);InsertChunkAtListHead(chunk);
    }
    if(chunk->NoAllocatedAreas==0){
        ++NoEmptyChunks;
        if(NoEmptyChunks>1)DeleteChunk(chunk);
    }
}
