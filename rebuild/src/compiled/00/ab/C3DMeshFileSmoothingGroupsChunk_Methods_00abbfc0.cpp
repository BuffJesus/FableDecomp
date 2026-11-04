// lib_3d_mesh_file_primitive.cpp, retail 0x00ABBFC0 / 0x00ABC500 / 0x00ABC980
// / 0x00ABEA90 / 0x00ABECB0 (Ego_r C3DMeshFileSmoothingGroupsChunk): the
// "SMTH" per-face smoothing groups (a count, the groups, then a zero word).
// Supersedes the stand-in GetChunkDescription landing at 0x00ABEA90.
#include <new>
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include "fable_stlport_vector_impl.h"

// Retail 00ABBFC0.
void C3DMeshFileSmoothingGroupsChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    const unsigned long count = FaceGroupsList.size();
    stream.WriteULONG(count);
    for (unsigned long i = 0; i < count; ++i)
        stream.WriteULONG(FaceGroupsList[i]);
    stream.WriteULONG(0);
}

// Retail 00ABC500.
CChunkedFileChunk* C3DMeshFileSmoothingGroupsChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileSmoothingGroupsChunk);
}

// Retail 00ABC980.
void C3DMeshFileSmoothingGroupsChunk::ReadFromFile(CDataInputStream& stream, unsigned long, CChunkedFileLoader&)
{
    const unsigned long count = stream.ReadULONG();
    FaceGroupsList.clear();
    FaceGroupsList.reserve(count);
    for (unsigned long i = 0; i < count; ++i)
        FaceGroupsList.push_back(stream.ReadULONG());
}

// Retail 00ABEA90.
const CCharString C3DMeshFileSmoothingGroupsChunk::GetChunkDescription() const
{
    return CCharString("Smoothing Groups");
}

C3DMeshFileSmoothingGroupsChunk::C3DMeshFileSmoothingGroupsChunk() : CChunkedFileChunk("SMTH") {}

C3DMeshFileSmoothingGroupsChunk::~C3DMeshFileSmoothingGroupsChunk()
{
}

// Retail 00AB9F80.
const CCharString C3DMeshFileSmoothingGroupsChunk::GetChunkInfo() const
{
    return CCharString("");
}
