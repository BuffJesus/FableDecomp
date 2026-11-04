// lib_3d_mesh_file_bone.cpp, retail 0x00AB9EA0-0x00AB9F5F, 0x00ABA170,
// 0x00ABADD0 and 0x00ABD400 (Ego_r C3DMeshFileBoneChunk): the "BONE" chunk
// with its hierarchy indices, localization matrix and (in newer files) name.
// Supersedes the stand-in GetChunkDescription landing at 0x00ABD400.
#include <stdio.h>
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"

// Retail 00AB9EA0.
void C3DMeshFileBoneChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    stream.WriteSLONG(BoneIndex);
    stream.WriteSLONG(ParentIndex);
    stream.WriteSLONG(FirstChildIndex);
    stream.WriteSLONG(NextSiblingIndex);
    stream.WriteMatrix3x4(LocalizationMatrix);
    stream.WriteNullTerminatedString(Name);
    CChunkedFileSuperChunk::WriteToFile(stream, saver);
}

// Retail 00AB9F00.
CChunkedFileChunk* C3DMeshFileBoneChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileBoneChunk);
}

// Retail 00ABA170: older files end the header before the name.
void C3DMeshFileBoneChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    const unsigned long start = stream.GetPosition();
    BoneIndex = stream.ReadValue<long>();
    ParentIndex = stream.ReadValue<long>();
    FirstChildIndex = stream.ReadValue<long>();
    NextSiblingIndex = stream.ReadValue<long>();
    LocalizationMatrix = stream.ReadMatrix3x4();
    if (stream.GetPosition() - start < size)
        Name = stream.ReadNullTerminatedString();
    const unsigned long bytes_read = stream.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(stream, size - bytes_read, loader);
}

// Retail 00ABADD0.
const CCharString C3DMeshFileBoneChunk::GetChunkInfo() const
{
    char text[1024];
    _snprintf(text, sizeof(text), "Name: %s, Index: %li, Parent: %li, Child: %li, Sibling: %li",
              Name.GetConstCharPtr(), BoneIndex, ParentIndex, FirstChildIndex, NextSiblingIndex);
    text[sizeof(text) - 1] = 0;
    return CCharString(text);
}

// Retail 00ABD400.
const CCharString C3DMeshFileBoneChunk::GetChunkDescription() const
{
    return CCharString("Bone");
}

C3DMeshFileBoneChunk::C3DMeshFileBoneChunk()
    : CChunkedFileSuperChunk("BONE"), BoneIndex(-1)
{
    ParentIndex = -1;
    FirstChildIndex = -1;
    NextSiblingIndex = -1;
}

// Retail 00AB9E30.
void C3DMeshFileBoneChunk::SetBoneIndex(long index)
{
    BoneIndex = index;
}

// Retail 00AB9E40.
void C3DMeshFileBoneChunk::SetParentIndex(long index)
{
    ParentIndex = index;
}

// Retail 00AB9E50.
void C3DMeshFileBoneChunk::SetFirstChildIndex(long index)
{
    FirstChildIndex = index;
}

// Retail 00AB9E60.
void C3DMeshFileBoneChunk::SetNextSiblingIndex(long index)
{
    NextSiblingIndex = index;
}

// Retail 00AB9E70.
void C3DMeshFileBoneChunk::SetLocalizationMatrix(const CMatrix3x4& matrix)
{
    LocalizationMatrix = matrix;
}

// Retail 00AB9E90.
void C3DMeshFileBoneChunk::SetName(const CCharString& name)
{
    Name = name;
}

// Retail 00AB9D50.
C3DMeshFileBoneChunk::C3DMeshFileBoneChunk(long bone_index, const CMatrix3x4& localization_matrix)
    : CChunkedFileSuperChunk("BONE"), BoneIndex(bone_index), LocalizationMatrix(localization_matrix)
{
    ParentIndex = -1;
    FirstChildIndex = -1;
    NextSiblingIndex = -1;
}
