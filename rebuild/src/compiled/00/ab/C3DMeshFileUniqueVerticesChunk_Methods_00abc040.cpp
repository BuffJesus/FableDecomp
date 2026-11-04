// lib_3d_mesh_file_primitive.cpp, retail 0x00ABC040 / 0x00ABC5F0 / 0x00ABCAF0
// / 0x00ABEAB0 / 0x00ABECF0 (Ego_r C3DMeshFileUniqueVerticesChunk): the
// "UNIV" list of unique vertex indices (a count, then the indices).
// Supersedes the stand-in GetChunkDescription landing at 0x00ABEAB0.
#include <new>
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include "fable_stlport_vector_impl.h"

// Retail 00ABC040.
void C3DMeshFileUniqueVerticesChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    const unsigned long count = UniqueVerticesList.size();
    stream.WriteULONG(count);
    for (unsigned long i = 0; i < count; ++i)
        stream.WriteULONG(UniqueVerticesList[i]);
}

// Retail 00ABC5F0.
CChunkedFileChunk* C3DMeshFileUniqueVerticesChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileUniqueVerticesChunk);
}

// Retail 00ABCAF0.
void C3DMeshFileUniqueVerticesChunk::ReadFromFile(CDataInputStream& stream, unsigned long, CChunkedFileLoader&)
{
    const unsigned long count = stream.ReadULONG();
    UniqueVerticesList.clear();
    UniqueVerticesList.reserve(count);
    for (unsigned long i = 0; i < count; ++i)
        UniqueVerticesList.push_back(stream.ReadULONG());
}

// Retail 00ABEAB0.
const CCharString C3DMeshFileUniqueVerticesChunk::GetChunkDescription() const
{
    return CCharString("Unique Vertex Information");
}

C3DMeshFileUniqueVerticesChunk::C3DMeshFileUniqueVerticesChunk() : CChunkedFileChunk("UNIV") {}

C3DMeshFileUniqueVerticesChunk::~C3DMeshFileUniqueVerticesChunk()
{
}

// Retail 00AB9FA0.
const CCharString C3DMeshFileUniqueVerticesChunk::GetChunkInfo() const
{
    return CCharString("");
}
