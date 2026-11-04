// Ego_r lib_3d_mesh_file_material.cpp: a material's map sub-chunks by type.
#include "fable_chunked_file.h"
#include "fable_counted_pointer_inline.h"

bool C3DMeshFileMaterialChunk::HasExtendedProperties() const
{
    return CountSubChunksOfType(C3DMeshFileMaterialExtChunk::GetStaticChunkType()) > 0;
}

// The first map of the type, or null.
CCountedPointer<C3DMeshFileMapChunk> C3DMeshFileMaterialChunk::GetMap(EMaterialMapType type)
{
    for (CCountedPointer<C3DMeshFileMapChunk> map = FindFirstMap(); map.Get(); map = FindNextMap(map))
    {
        if (map->GetType() == type)
            return map;
    }
    return CCountedPointer<C3DMeshFileMapChunk>(0);
}

CCountedPointer<const C3DMeshFileMapChunk> C3DMeshFileMaterialChunk::GetMap(EMaterialMapType type) const
{
    for (CCountedPointer<const C3DMeshFileMapChunk> map = FindFirstMap(); map.Get(); map = FindNextMap(map))
    {
        if (map->GetType() == type)
            return map;
    }
    return CCountedPointer<const C3DMeshFileMapChunk>(0);
}
