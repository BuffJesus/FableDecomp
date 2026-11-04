// Ego_r lib_3d_mesh_file_submesh.cpp: the list accessors of the primitive
// sub-chunks (vertex groups, vertices, triangles, smoothing groups, unique
// vertices) and the primitive's material indices.
#include <new>
#include "fable_chunked_file.h"
#include "fable_stlport_vector_impl.h"

long C3DMeshFileGroupChunk::GetBoneIndex() const
{
    return BoneIndex;
}

void C3DMeshFileGroupChunk::SetBoneIndex(long index)
{
    BoneIndex = index;
}

unsigned long C3DMeshFileGroupChunk::GetCount() const
{
    return VertexList.size();
}

unsigned long C3DMeshFileVerticesChunk::GetCount() const
{
    return VertexList.size();
}

unsigned long C3DMeshFileTrianglesChunk::GetCount() const
{
    return TriangleList.size();
}

unsigned long C3DMeshFileUniqueVerticesChunk::FindHighestUniqueVertexIndex() const
{
    unsigned long highest = 0;
    const unsigned long count = UniqueVerticesList.size();
    for (unsigned long i = 0; i < count; ++i)
        highest = highest > UniqueVerticesList[i] ? highest : UniqueVerticesList[i];
    return highest;
}

const C3DVertexBlend2* C3DMeshFileGroupChunk::GetList() const
{
    return VertexList.GetAsCArray();
}

C3DVertexBlend2* C3DMeshFileGroupChunk::GetList()
{
    return VertexList.GetAsCArray();
}

const C3DVertex2* C3DMeshFileVerticesChunk::GetList() const
{
    return VertexList.GetAsCArray();
}

C3DVertex2* C3DMeshFileVerticesChunk::GetList()
{
    return VertexList.GetAsCArray();
}

const C3DTriangle2* C3DMeshFileTrianglesChunk::GetList() const
{
    return TriangleList.GetAsCArray();
}

C3DTriangle2* C3DMeshFileTrianglesChunk::GetList()
{
    return TriangleList.GetAsCArray();
}

void C3DMeshFileSmoothingGroupsChunk::ClearList()
{
    FaceGroupsList.clear();
}

void C3DMeshFileUniqueVerticesChunk::ClearList()
{
    UniqueVerticesList.clear();
}

void C3DMeshFilePrimitiveChunk::AddMaterialIndex(long index)
{
    MaterialIndices.push_back(index);
}

void C3DMeshFileSmoothingGroupsChunk::AddFaceGroupMask(unsigned long mask)
{
    FaceGroupsList.push_back(mask);
}

void C3DMeshFileUniqueVerticesChunk::AddUniqueVertexIndex(unsigned long index)
{
    UniqueVerticesList.push_back(index);
}

void C3DMeshFileTrianglesChunk::ClearList()
{
    TriangleList.clear();
}

void C3DMeshFileTrianglesChunk::AddTriangle(const C3DTriangle2& triangle)
{
    TriangleList.push_back(triangle);
}
