// lib_3d_anim_file_sequence.cpp, retail 0x00AAF5B0 / 0x00AAF650 / 0x00AAF7A0
// / 0x00AAFF90 (Ego_r C3DAnimFileSequenceChunk): the "ASEQ" per-bone
// animation sequence (bone, parent, name, sample rate, one matrix per frame
// and the bone type).
// Supersedes the stand-in GetChunkDescription landing at 0x00AAFF90.
#include <new>
#include <stdio.h>
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include "fable_stlport_vector_impl.h"

// CMatrix3x4 has constructors in Ego_r: its vector erase is out of line
// (retail 0x00AAFD10).
namespace _STL {
template<> CMatrix3x4* vector<CMatrix3x4>::erase(CMatrix3x4* __first, CMatrix3x4* __last);
}

// Retail 00AAF5B0.
const CCharString C3DAnimFileSequenceChunk::GetChunkInfo() const
{
    char text[1024];
    _snprintf(text, sizeof(text) - 1, "Bone Name: %s. Bone Index: %li, Frames: %lu, Framerate: %f",
              BoneName.GetConstCharPtr(), BoneIndex, Matrices.size(), SamplesPerSecond);
    text[sizeof(text) - 1] = 0;
    return CCharString(text);
}

// Retail 00AAF650.
void C3DAnimFileSequenceChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    stream.WriteSLONG(BoneIndex);
    stream.WriteSLONG(ParentIndex);
    stream.WriteNullTerminatedString(BoneName);
    stream.WriteFloat(SamplesPerSecond);
    const unsigned long count = Matrices.size();
    stream.WriteSLONG(count);
    for (unsigned long i = 0; i < count; ++i)
        stream.WriteMatrix3x4(Matrices[i]);
    stream.WriteUBYTE((unsigned char)BoneType);
    CChunkedFileSuperChunk::WriteToFile(stream, saver);
}

// Retail 00AAF7A0.
CChunkedFileChunk* C3DAnimFileSequenceChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DAnimFileSequenceChunk);
}

// Retail 00AAF850: the bone type was added later (absent in older files).
void C3DAnimFileSequenceChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    const unsigned long end = stream.GetPosition() + size;
    BoneIndex = stream.ReadValue<long>();
    ParentIndex = stream.ReadValue<long>();
    BoneName = stream.ReadNullTerminatedString();
    SamplesPerSecond = stream.ReadFloat();
    Matrices.clear();
    Matrices.reserve(0);
    const unsigned long count = stream.ReadULONG();
    for (unsigned long i = 0; i < count; ++i)
    {
        const CMatrix3x4 matrix = stream.ReadMatrix3x4();
        Matrices.push_back(matrix);
    }
    BoneType = BONE_TYPE_DEFAULT;
    if (stream.GetPosition() < end)
        BoneType = (EBoneType)stream.ReadValue<unsigned char>();
    if (stream.GetPosition() < end)
        CChunkedFileSuperChunk::ReadFromFile(stream, end - stream.GetPosition(), loader);
}

// Retail 00AAFF90.
const CCharString C3DAnimFileSequenceChunk::GetChunkDescription() const
{
    return CCharString("Animation Sequence");
}

C3DAnimFileSequenceChunk::C3DAnimFileSequenceChunk()
    : CChunkedFileSuperChunk("ASEQ"), BoneIndex(-1), BoneType(BONE_TYPE_DEFAULT)
{
}
