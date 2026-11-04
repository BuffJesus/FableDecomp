#include "candidates/fixed_size_allocator_accessors_compiled.h"

CMemoryAllocatorFixedSize::CMemoryAllocatorFixedSize(void*,
    unsigned long minimumChunkSize, unsigned long areaSize,
    unsigned long alignment, unsigned long flags, unsigned long internalFlags)
    : CMemoryAllocatorBase(alignment),
      PCurrentAllocatorChunk(0), ExtraMemUsedByOS(0), NoEmptyChunks(0),
      AreaSize(((areaSize + alignment + 3) / alignment) * alignment - 4),
      ChunkHeaderSize((((Alignment + 0x1b) >> AlignmentShift) << AlignmentShift) - 4)
{
    PFirstChunk = 0;
    PLastChunk = 0;
    StatAreaSize = AreaSize + 4;
    NoAreasPerChunk = (minimumChunkSize - ChunkHeaderSize) / StatAreaSize;
    ChunkSize = NoAreasPerChunk * StatAreaSize + ChunkHeaderSize;
    Flags = flags;
    InternalFlags = internalFlags;
}
