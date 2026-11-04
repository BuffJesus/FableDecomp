// lib_3d_mesh_file_submesh.cpp, retail 0x00AB9AF0-0x00ABA42F and 0x00ABD3C0
// (Ego_r C3DMeshFileSubMeshChunk): the "SUBM" sub-mesh chunk carrying a
// name and its hierarchy indices (submesh, parent, first child, next sibling).
#include <stdio.h>
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"

// Retail 00AB9AF0: name, the four indices, then the sub-chunks.
void C3DMeshFileSubMeshChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    stream.WriteNullTerminatedString(Name);
    stream.WriteSLONG(SubMeshIndex);
    stream.WriteSLONG(ParentIndex);
    stream.WriteSLONG(FirstChildIndex);
    stream.WriteSLONG(NextSiblingIndex);
    CChunkedFileSuperChunk::WriteToFile(stream, saver);
}

// Retail 00AB9B50.
CChunkedFileChunk* C3DMeshFileSubMeshChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileSubMeshChunk);
}

// Retail 00AB9FC0: the header fields, then the sub-chunks in what remains.
void C3DMeshFileSubMeshChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    const unsigned long start = stream.GetPosition();
    Name = stream.ReadNullTerminatedString();
    SubMeshIndex = stream.ReadValue<long>();
    ParentIndex = stream.ReadValue<long>();
    FirstChildIndex = stream.ReadValue<long>();
    NextSiblingIndex = stream.ReadValue<long>();
    const unsigned long bytes_read = stream.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(stream, size - bytes_read, loader);
}

// Retail 00ABA390: a one-line summary for the chunk viewer.
const CCharString C3DMeshFileSubMeshChunk::GetChunkInfo() const
{
    char text[1024];
    _snprintf(text, sizeof(text), "Name: %s, Index: %li, Parent: %li, Child: %li, Sibling: %li",
              Name.GetConstCharPtr(), SubMeshIndex, ParentIndex, FirstChildIndex, NextSiblingIndex);
    text[sizeof(text) - 1] = 0;
    return CCharString(text);
}

// Retail 00ABD3C0.
const CCharString C3DMeshFileSubMeshChunk::GetChunkDescription() const
{
    return CCharString("Sub-Mesh");
}

C3DMeshFileSubMeshChunk::C3DMeshFileSubMeshChunk()
    : CChunkedFileSuperChunk("SUBM"), SubMeshIndex(-1), ParentIndex(-1), FirstChildIndex(-1), NextSiblingIndex(-1)
{
}

// Retail 00AB9AA0.
void C3DMeshFileSubMeshChunk::SetName(const CCharString& name)
{
    Name = name;
}

// Retail 00AB9AB0.
void C3DMeshFileSubMeshChunk::SetSubMeshIndex(long index)
{
    SubMeshIndex = index;
}

// Retail 00AB9AC0.
void C3DMeshFileSubMeshChunk::SetParentIndex(long index)
{
    ParentIndex = index;
}

// Retail 00AB9AD0.
void C3DMeshFileSubMeshChunk::SetFirstChildIndex(long index)
{
    FirstChildIndex = index;
}

// Retail 00AB9AE0.
void C3DMeshFileSubMeshChunk::SetNextSiblingIndex(long index)
{
    NextSiblingIndex = index;
}
