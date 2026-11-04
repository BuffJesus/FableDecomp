// lib_3d_mesh_file_helpers.cpp, retail 0x00AA96B0 / 0x00AA98B0 / 0x00AA9DA0
// / 0x00AAB130 (Ego_r C3DMeshFileHelperPointChunk): the "HPNT" named helper
// point (position, sub-mesh and bone) ahead of its sub-chunks.
// Supersedes the stand-in
// GetChunkDescription landing at 0x00AAB130.
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"

// Retail 00AA96B0.
const CCharString C3DMeshFileHelperPointChunk::GetChunkInfo() const
{
    return CCharString("Name: ") + Name + ", Position: (" + GFFloatToCharString(Position.X, 6) + ", " +
           GFFloatToCharString(Position.Y, 6) + ", " + GFFloatToCharString(Position.Z, 6) + "), Sub-mesh: " +
           GFIntToCharString(SubMeshIndex) + ", Bone: " + GFIntToCharString(BoneIndex);
}

// Retail 00AA98B0.
void C3DMeshFileHelperPointChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    stream.WriteFloat(Position.X);
    stream.WriteFloat(Position.Y);
    stream.WriteFloat(Position.Z);
    stream.WriteSLONG(SubMeshIndex);
    stream.WriteSLONG(BoneIndex);
    stream.WriteNullTerminatedString(Name);
    CChunkedFileSuperChunk::WriteToFile(stream, saver);
}

// Retail 00AA9BA0.
void C3DMeshFileHelperPointChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    const unsigned long start = stream.GetPosition();
    Position.X = stream.ReadFloat();
    Position.Y = stream.ReadFloat();
    Position.Z = stream.ReadFloat();
    SubMeshIndex = stream.ReadValue<long>();
    BoneIndex = stream.ReadValue<long>();
    Name = stream.ReadNullTerminatedString();
    const unsigned long bytes_read = stream.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(stream, size - bytes_read, loader);
}

// Retail 00AA9DA0.
CChunkedFileChunk* C3DMeshFileHelperPointChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileHelperPointChunk);
}

// Retail 00AAB130.
const CCharString C3DMeshFileHelperPointChunk::GetChunkDescription() const
{
    return CCharString("Helper Point");
}

C3DMeshFileHelperPointChunk::C3DMeshFileHelperPointChunk()
    : CChunkedFileSuperChunk("HPNT"), Position(0.0f, 0.0f, 0.0f), SubMeshIndex(-1), BoneIndex(-1)
{
}

// Retail 00AA9860.
void C3DMeshFileHelperPointChunk::SetPosition(const C3DVector& position)
{
    Position = position;
}

// Retail 00AA9880.
void C3DMeshFileHelperPointChunk::SetSubMeshIndex(long index)
{
    SubMeshIndex = index;
}

// Retail 00AA9890.
void C3DMeshFileHelperPointChunk::SetBoneIndex(long index)
{
    BoneIndex = index;
}

// Retail 00AA98A0.
void C3DMeshFileHelperPointChunk::SetName(const CCharString& name)
{
    Name = name;
}
