// lib_3d_mesh_file_primitive.cpp, retail 0x00ABAE50 / 0x00ABAEE0 / 0x00ABC6E0
// / 0x00ABEC50 / 0x00ABEDD0 (Ego_r C3DMeshFileGroupChunk): the "VGRP" vertex
// group of one bone (vertex index stored as 16 bits plus blend weight).
// ReadFromFile 0x00ABCC80 is a near draft (register allocation only; see the
// 2026-10-16 journal). Supersedes the stand-in
// GetChunkDescription landing at 0x00ABEC50.
#include <stdio.h>
#include "fable_chunked_file.h"
#include "fable_data_output_stream.h"
#include "fable_stlport_algobase.h"
#include <new>
#include "fable_stlport_vector_impl.h"

// C3DVertexBlend2 has a constructor, so STLport erases its vector element by
// element (__copy expands inline; ClearList 0x00ABC6B0).
namespace _STL {
template<> inline C3DVertexBlend2* vector<C3DVertexBlend2>::erase(C3DVertexBlend2* __first, C3DVertexBlend2* __last)
{
    C3DVertexBlend2* __i = __copy(__last, this->_M_finish, __first, random_access_iterator_tag(), (ptrdiff_t*)0);
    this->_M_finish = __i;
    return __first;
}
}

// Retail 00ABAE50.
const CCharString C3DMeshFileGroupChunk::GetChunkInfo() const
{
    char text[1024];
    sprintf(text, "Bone Index: %li, Vertex Count: %lu", BoneIndex, VertexList.size());
    return CCharString(text);
}

// Retail 00ABAEE0.
void C3DMeshFileGroupChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    stream.WriteSLONG(BoneIndex);
    const unsigned long count = VertexList.size();
    stream.WriteULONG(count);
    for (unsigned long i = 0; i < count; ++i)
    {
        const C3DVertexBlend2& blend = VertexList[i];
        stream.WriteUWORD((unsigned short)blend.VertexIndex);
        stream.WriteFloat(blend.BlendValue);
    }
    CChunkedFileSuperChunk::WriteToFile(stream, saver);
}

// Retail 00ABC6E0.
CChunkedFileChunk* C3DMeshFileGroupChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileGroupChunk);
}

// Retail 00ABEC50.
const CCharString C3DMeshFileGroupChunk::GetChunkDescription() const
{
    return CCharString("Vertex Group");
}

C3DMeshFileGroupChunk::C3DMeshFileGroupChunk() : CChunkedFileSuperChunk("VGRP")
{
    BoneIndex = -1;
}

C3DMeshFileGroupChunk::~C3DMeshFileGroupChunk()
{
}

// Retail 00ABC6B0.
void C3DMeshFileGroupChunk::ClearList()
{
    VertexList.clear();
}

// Retail 00ABCC20.
// A repeat of the last vertex adds its weight to that entry.
void C3DMeshFileGroupChunk::AddVertex(const C3DVertexBlend2& vertex)
{
    if (!VertexList.empty() && VertexList.back().GetVertexIndex() == vertex.GetVertexIndex())
    {
        C3DVertexBlend2& last = VertexList.back();
        last.SetBlendValue(vertex.GetBlendValue() + last.GetBlendValue());
    }
    else
        VertexList.push_back(vertex);
}
