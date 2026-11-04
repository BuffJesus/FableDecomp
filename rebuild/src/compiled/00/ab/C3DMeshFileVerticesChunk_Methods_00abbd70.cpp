// lib_3d_mesh_file_primitive.cpp, retail 0x00ABBD70 / 0x00ABBDF0 / 0x00ABC800
// / 0x00ABD150 / 0x00ABEC70 (Ego_r C3DMeshFileVerticesChunk): the "VERT"
// vertex list, written field by field but read back as one block.
// Supersedes the stand-in GetChunkDescription landing at 0x00ABEC70.
#include <new>
#include <stdio.h>
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include "fable_stlport_vector_impl.h"

// C3DVertex2 has a constructor, so its vector erase is the out-of-line
// specialization (retail 0x00A96A90); resize is inlined here (retail calls
// erase or _M_fill_insert 0x00A97040 directly).
namespace _STL {
template<> C3DVertex2* vector<C3DVertex2>::erase(C3DVertex2* __first, C3DVertex2* __last);
template<> inline void vector<C3DVertex2>::resize(size_t __new_size, const C3DVertex2& __x)
{
    if (__new_size < size())
        erase(begin() + __new_size, end());
    else
        insert(end(), __new_size - size(), __x);
}
}

// Retail 00ABBD70.
const CCharString C3DMeshFileVerticesChunk::GetChunkInfo() const
{
    char text[128];
    sprintf(text, "Number of vertices: %u", VertexList.size());
    return CCharString(text);
}

// Retail 00ABBDF0.
void C3DMeshFileVerticesChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    const unsigned long count = VertexList.size();
    stream.WriteULONG(count);
    for (unsigned long i = 0; i < count; ++i)
    {
        const C3DVertex2& vertex = VertexList[i];
        stream.WriteFloat(vertex.Position.X);
        stream.WriteFloat(vertex.Position.Y);
        stream.WriteFloat(vertex.Position.Z);
        stream.WriteFloat(vertex.Normal.X);
        stream.WriteFloat(vertex.Normal.Y);
        stream.WriteFloat(vertex.Normal.Z);
        stream.WriteFloat(vertex.U);
        stream.WriteFloat(vertex.V);
    }
}

// Retail 00ABC800.
CChunkedFileChunk* C3DMeshFileVerticesChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileVerticesChunk);
}

// Retail 00ABD150.
void C3DMeshFileVerticesChunk::ReadFromFile(CDataInputStream& stream, unsigned long, CChunkedFileLoader&)
{
    const unsigned long count = stream.ReadULONG();
    VertexList.resize(count);
    stream.Read(VertexList.GetAsCArray(), VertexList.size() * sizeof(C3DVertex2));
}

// Retail 00ABEC70.
const CCharString C3DMeshFileVerticesChunk::GetChunkDescription() const
{
    return CCharString("Vertex List");
}

C3DMeshFileVerticesChunk::C3DMeshFileVerticesChunk() : CChunkedFileChunk("VERT") {}

// Retail 00ABC7B0.
C3DMeshFileVerticesChunk::~C3DMeshFileVerticesChunk()
{
}

// Retail 00ABC7E0.
void C3DMeshFileVerticesChunk::ClearList()
{
    VertexList.clear();
}

void C3DMeshFileVerticesChunk::AddVertex(const C3DVertex2& vertex)
{
    VertexList.push_back(vertex);
}
