// lib_3d_mesh_file_primitive.cpp, retail 0x00ABBE90 / 0x00ABBF30 / 0x00ABC8E0
// / 0x00ABD280 / 0x00ABEC90 (Ego_r C3DMeshFileTrianglesChunk): the "TRIS"
// triangle list (three 16-bit vertex indices each) ahead of its sub-chunks.
// Supersedes the stand-in GetChunkDescription landing at 0x00ABEC90.
#include <new>
#include <stdio.h>
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include "fable_stlport_vector_impl.h"

// C3DTriangle2 has a constructor; as for C3DVertex2, erase is out of line and
// resize is inlined.
namespace _STL {
template<> C3DTriangle2* vector<C3DTriangle2>::erase(C3DTriangle2* __first, C3DTriangle2* __last);
template<> inline void vector<C3DTriangle2>::resize(size_t __new_size, const C3DTriangle2& __x)
{
    if (__new_size < size())
        erase(begin() + __new_size, end());
    else
        insert(end(), __new_size - size(), __x);
}
}

// Retail 00ABBE90.
const CCharString C3DMeshFileTrianglesChunk::GetChunkInfo() const
{
    char text[128];
    sprintf(text, "Number of triangles: %u", TriangleList.size());
    return CCharString(text);
}

// Retail 00ABBF30.
void C3DMeshFileTrianglesChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    const unsigned long count = TriangleList.size();
    stream.WriteULONG(count);
    for (unsigned long i = 0; i < count; ++i)
    {
        const C3DTriangle2& triangle = TriangleList[i];
        stream.WriteUWORD(triangle.VertexIndex[0]);
        stream.WriteUWORD(triangle.VertexIndex[1]);
        stream.WriteUWORD(triangle.VertexIndex[2]);
    }
    CChunkedFileSuperChunk::WriteToFile(stream, saver);
}

// Retail 00ABC8E0.
CChunkedFileChunk* C3DMeshFileTrianglesChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileTrianglesChunk);
}

// Retail 00ABD280.
void C3DMeshFileTrianglesChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    const unsigned long start = stream.GetPosition();
    const unsigned long count = stream.ReadULONG();
    TriangleList.resize(count);
    stream.Read(TriangleList.GetAsCArray(), TriangleList.size() * sizeof(C3DTriangle2));
    const unsigned long bytes_read = stream.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(stream, size - bytes_read, loader);
}

// Retail 00ABEC90.
const CCharString C3DMeshFileTrianglesChunk::GetChunkDescription() const
{
    return CCharString("Triangle List");
}

C3DMeshFileTrianglesChunk::C3DMeshFileTrianglesChunk() : CChunkedFileSuperChunk("TRIS") {}

// Retail 00ABC890.
C3DMeshFileTrianglesChunk::~C3DMeshFileTrianglesChunk()
{
}
