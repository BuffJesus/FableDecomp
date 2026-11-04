// lib_3d_anim_file_bone_mask.cpp, retail 0x00AB0110, 0x00AB06B0-0x00AB086F
// and 0x00AB0B30 (Ego_r C3DAnimFileBoneMaskChunk): the "AMSK" per-bone enable
// mask, stored as a count then one byte per bone. Supersedes the stand-in
// landings at 0x00AB0110 and 0x00AB0B30.
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"

// Retail 00AB0110.
const CCharString C3DAnimFileBoneMaskChunk::GetChunkInfo() const
{
    return CCharString("");
}

// Retail 00AB06B0.
void C3DAnimFileBoneMaskChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    stream.WriteULONG(EnableMask.size());
    for (unsigned long i = 0; i < EnableMask.size(); ++i)
        stream.WriteEBOOL(EnableMask[i]);
}

// Retail 00AB0730.
CChunkedFileChunk* C3DAnimFileBoneMaskChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DAnimFileBoneMaskChunk);
}

// Retail 00AB0790.
void C3DAnimFileBoneMaskChunk::ReadFromFile(CDataInputStream& stream, unsigned long, CChunkedFileLoader&)
{
    const unsigned long count = stream.ReadULONG();
    EnableMask.resize(count, false);
    for (unsigned long i = 0; i < count; ++i)
        EnableMask[i] = stream.ReadEBOOL();
}

// Retail 00AB0B30.
const CCharString C3DAnimFileBoneMaskChunk::GetChunkDescription() const
{
    return CCharString("Animation Mask");
}

C3DAnimFileBoneMaskChunk::C3DAnimFileBoneMaskChunk() : CChunkedFileChunk("AMSK") {}
