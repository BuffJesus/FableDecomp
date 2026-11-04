// lib_3d_mesh_file_submesh.cpp, retail 0x00ABA4A0-0x00ABBD70 (Ego_r
// C3DMeshFileSubMeshChunk / C3DMeshFilePrimitiveChunk typed finders): each
// shares the base class's search result as the derived chunk type
// (CCountedPointer::SetStaticCast, as in FableWin) and returns it by value.
#include "fable_chunked_file.h"
#include "fable_counted_pointer_inline.h"

CCountedPointer<C3DMeshFileTransformationChunk> C3DMeshFileSubMeshChunk::FindTransformation()
{
    CCountedPointer<C3DMeshFileTransformationChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileTransformationChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABA4A0.
CCountedPointer<const C3DMeshFileTransformationChunk> C3DMeshFileSubMeshChunk::FindTransformation() const
{
    CCountedPointer<const C3DMeshFileTransformationChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileTransformationChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABA530.
CCountedPointer<C3DMeshFileBoneChunk> C3DMeshFileSubMeshChunk::FindFirstBone()
{
    CCountedPointer<C3DMeshFileBoneChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileBoneChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABA5C0.
CCountedPointer<C3DMeshFileBoneChunk> C3DMeshFileSubMeshChunk::FindNextBone(const CCountedPointer<const C3DMeshFileBoneChunk>& previous)
{
    CCountedPointer<C3DMeshFileBoneChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00ABA6A0.
CCountedPointer<const C3DMeshFileBoneChunk> C3DMeshFileSubMeshChunk::FindFirstBone() const
{
    CCountedPointer<const C3DMeshFileBoneChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileBoneChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABA730.
CCountedPointer<const C3DMeshFileBoneChunk> C3DMeshFileSubMeshChunk::FindNextBone(const CCountedPointer<const C3DMeshFileBoneChunk>& previous) const
{
    CCountedPointer<const C3DMeshFileBoneChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00ABA810.
CCountedPointer<C3DMeshFilePrimitiveChunk> C3DMeshFileSubMeshChunk::FindFirstPrimitive()
{
    CCountedPointer<C3DMeshFilePrimitiveChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFilePrimitiveChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABA8A0.
CCountedPointer<C3DMeshFilePrimitiveChunk> C3DMeshFileSubMeshChunk::FindNextPrimitive(const CCountedPointer<const C3DMeshFilePrimitiveChunk>& previous)
{
    CCountedPointer<C3DMeshFilePrimitiveChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00ABA980.
CCountedPointer<const C3DMeshFilePrimitiveChunk> C3DMeshFileSubMeshChunk::FindFirstPrimitive() const
{
    CCountedPointer<const C3DMeshFilePrimitiveChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFilePrimitiveChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABAA10.
CCountedPointer<const C3DMeshFilePrimitiveChunk> C3DMeshFileSubMeshChunk::FindNextPrimitive(const CCountedPointer<const C3DMeshFilePrimitiveChunk>& previous) const
{
    CCountedPointer<const C3DMeshFilePrimitiveChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00ABAAF0.
CCountedPointer<C3DMeshFileClothChunk> C3DMeshFileSubMeshChunk::FindFirstCloth()
{
    CCountedPointer<C3DMeshFileClothChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileClothChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABAB80.
CCountedPointer<C3DMeshFileClothChunk> C3DMeshFileSubMeshChunk::FindNextCloth(const CCountedPointer<const C3DMeshFileClothChunk>& previous)
{
    CCountedPointer<C3DMeshFileClothChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00ABAC60.
CCountedPointer<const C3DMeshFileClothChunk> C3DMeshFileSubMeshChunk::FindFirstCloth() const
{
    CCountedPointer<const C3DMeshFileClothChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileClothChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABACF0.
CCountedPointer<const C3DMeshFileClothChunk> C3DMeshFileSubMeshChunk::FindNextCloth(const CCountedPointer<const C3DMeshFileClothChunk>& previous) const
{
    CCountedPointer<const C3DMeshFileClothChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00ABAF50.
CCountedPointer<C3DMeshFileVerticesChunk> C3DMeshFilePrimitiveChunk::GetVertices()
{
    CCountedPointer<C3DMeshFileVerticesChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileVerticesChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABAFE0.
CCountedPointer<const C3DMeshFileVerticesChunk> C3DMeshFilePrimitiveChunk::PeekVertices() const
{
    CCountedPointer<const C3DMeshFileVerticesChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileVerticesChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABB070.
CCountedPointer<C3DMeshFileTrianglesChunk> C3DMeshFilePrimitiveChunk::GetTriangles()
{
    CCountedPointer<C3DMeshFileTrianglesChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileTrianglesChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABB100.
CCountedPointer<const C3DMeshFileTrianglesChunk> C3DMeshFilePrimitiveChunk::PeekTriangles() const
{
    CCountedPointer<const C3DMeshFileTrianglesChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileTrianglesChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABB190.
CCountedPointer<C3DMeshFileTrianglesChunk> C3DMeshFilePrimitiveChunk::FindFirstTriangles()
{
    CCountedPointer<C3DMeshFileTrianglesChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileTrianglesChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABB220.
CCountedPointer<C3DMeshFileTrianglesChunk> C3DMeshFilePrimitiveChunk::FindNextTriangles(const CCountedPointer<const C3DMeshFileTrianglesChunk>& previous)
{
    CCountedPointer<C3DMeshFileTrianglesChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00ABB300.
CCountedPointer<const C3DMeshFileTrianglesChunk> C3DMeshFilePrimitiveChunk::FindFirstTriangles() const
{
    CCountedPointer<const C3DMeshFileTrianglesChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileTrianglesChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABB390.
CCountedPointer<const C3DMeshFileTrianglesChunk> C3DMeshFilePrimitiveChunk::FindNextTriangles(const CCountedPointer<const C3DMeshFileTrianglesChunk>& previous) const
{
    CCountedPointer<const C3DMeshFileTrianglesChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00ABB470.
bool C3DMeshFilePrimitiveChunk::HasSmoothingGroups() const
{
    CCountedPointer<const C3DMeshFileSmoothingGroupsChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileSmoothingGroupsChunk::GetStaticChunkType()));
    return !chunk.IsNull();
}

// Retail 00ABB4F0.
CCountedPointer<C3DMeshFileSmoothingGroupsChunk> C3DMeshFilePrimitiveChunk::GetSmoothingGroups()
{
    CCountedPointer<C3DMeshFileSmoothingGroupsChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileSmoothingGroupsChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABB580.
CCountedPointer<const C3DMeshFileSmoothingGroupsChunk> C3DMeshFilePrimitiveChunk::PeekSmoothingGroups() const
{
    CCountedPointer<const C3DMeshFileSmoothingGroupsChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileSmoothingGroupsChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABB610.
bool C3DMeshFilePrimitiveChunk::HasUniqueVertices() const
{
    CCountedPointer<const C3DMeshFileUniqueVerticesChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileUniqueVerticesChunk::GetStaticChunkType()));
    return !chunk.IsNull();
}

// Retail 00ABB690.
CCountedPointer<C3DMeshFileUniqueVerticesChunk> C3DMeshFilePrimitiveChunk::GetUniqueVertices()
{
    CCountedPointer<C3DMeshFileUniqueVerticesChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileUniqueVerticesChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABB720.
CCountedPointer<const C3DMeshFileUniqueVerticesChunk> C3DMeshFilePrimitiveChunk::PeekUniqueVertices() const
{
    CCountedPointer<const C3DMeshFileUniqueVerticesChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileUniqueVerticesChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABB7B0.
CCountedPointer<C3DMeshFileGroupChunk> C3DMeshFilePrimitiveChunk::FindFirstGroup()
{
    CCountedPointer<C3DMeshFileGroupChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileGroupChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABB840.
CCountedPointer<C3DMeshFileGroupChunk> C3DMeshFilePrimitiveChunk::FindNextGroup(const CCountedPointer<const C3DMeshFileGroupChunk>& previous)
{
    CCountedPointer<C3DMeshFileGroupChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00ABB920.
CCountedPointer<const C3DMeshFileGroupChunk> C3DMeshFilePrimitiveChunk::FindFirstGroup() const
{
    CCountedPointer<const C3DMeshFileGroupChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileGroupChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABB9B0.
CCountedPointer<const C3DMeshFileGroupChunk> C3DMeshFilePrimitiveChunk::FindNextGroup(const CCountedPointer<const C3DMeshFileGroupChunk>& previous) const
{
    CCountedPointer<const C3DMeshFileGroupChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00ABBA90.
CCountedPointer<C3DMeshFileClothChunk> C3DMeshFilePrimitiveChunk::FindFirstCloth()
{
    CCountedPointer<C3DMeshFileClothChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileClothChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABBB20.
CCountedPointer<C3DMeshFileClothChunk> C3DMeshFilePrimitiveChunk::FindNextCloth(const CCountedPointer<const C3DMeshFileClothChunk>& previous)
{
    CCountedPointer<C3DMeshFileClothChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}

// Retail 00ABBC00.
CCountedPointer<const C3DMeshFileClothChunk> C3DMeshFilePrimitiveChunk::FindFirstCloth() const
{
    CCountedPointer<const C3DMeshFileClothChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileClothChunk::GetStaticChunkType()));
    return chunk;
}

// Retail 00ABBC90.
CCountedPointer<const C3DMeshFileClothChunk> C3DMeshFilePrimitiveChunk::FindNextCloth(const CCountedPointer<const C3DMeshFileClothChunk>& previous) const
{
    CCountedPointer<const C3DMeshFileClothChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}
