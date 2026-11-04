// lib_3d_anim_file_object.cpp, retail 0x00AB0090 / 0x00AB00C0 / 0x00AB0130 /
// 0x00AB01E0 / 0x00AB0870 (Ego_r C3DAnimFileObjectChunk): the "AOBJ" chunk
// naming an animated object and its sub-mesh; only the name and sub-mesh
// index are serialised. Supersedes the stand-in GetChunkDescription landing.
#include <stdio.h>
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"

// Retail 00AB0090.
void C3DAnimFileObjectChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    stream.WriteNullTerminatedString(Name);
    stream.WriteSLONG(SubMeshIndex);
    CChunkedFileSuperChunk::WriteToFile(stream, saver);
}

// Retail 00AB00C0.
CChunkedFileChunk* C3DAnimFileObjectChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DAnimFileObjectChunk);
}

// Retail 00AB0130.
void C3DAnimFileObjectChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    const unsigned long start = stream.GetPosition();
    Name = stream.ReadNullTerminatedString();
    SubMeshIndex = stream.ReadValue<long>();
    const unsigned long bytes_read = stream.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(stream, size - bytes_read, loader);
}

// Retail 00AB01E0.
const CCharString C3DAnimFileObjectChunk::GetChunkInfo() const
{
    char text[1024];
    _snprintf(text, sizeof(text), "Name: %s, Index: %li, Parent: %li, Child: %li, Sibling: %li",
              Name.GetConstCharPtr(), SubMeshIndex, ParentIndex, FirstChildIndex, NextSiblingIndex);
    text[sizeof(text) - 1] = 0;
    return CCharString(text);
}

// Retail 00AB0870.
const CCharString C3DAnimFileObjectChunk::GetChunkDescription() const
{
    return CCharString("Animated Object");
}

C3DAnimFileObjectChunk::C3DAnimFileObjectChunk() : CChunkedFileSuperChunk("AOBJ") {}

void C3DAnimFileObjectChunk::SetName(const CCharString& name)
{
    Name = name;
}

void C3DAnimFileObjectChunk::SetSubMeshIndex(long index)
{
    SubMeshIndex = index;
}

void C3DAnimFileObjectChunk::SetParentIndex(long index)
{
    ParentIndex = index;
}

void C3DAnimFileObjectChunk::SetFirstChildIndex(long index)
{
    FirstChildIndex = index;
}

void C3DAnimFileObjectChunk::SetNextSiblingIndex(long index)
{
    NextSiblingIndex = index;
}
