// lib_3d_anim_file_x.cpp, retail 0x00AA4650 / 0x00AA46B0 / 0x00AA48D0 /
// 0x00AA49D0 (Ego_r C3DAnimFileXSequenceChunk): the "XSEQ"
// chunk carrying a compressed animation sequence as raw bytes (the whole
// chunk body). The scalar deleting destructor 0x00AA49F0 (inlined destructor
// without a vptr reset) is not reproduced yet. Supersedes the stand-in GetChunkDescription landing.
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include "fable_stlport_vector_impl.h"

// Retail 00AA4650.
void C3DAnimFileXSequenceChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    stream.Write(CompressedData.GetAsCArray(), CompressedData.size());
}

// Retail 00AA46B0.
CChunkedFileChunk* C3DAnimFileXSequenceChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DAnimFileXSequenceChunk);
}

// Retail 00AA48D0.
void C3DAnimFileXSequenceChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader&)
{
    CompressedData.resize(size, 0);
    stream.Read(CompressedData.GetAsCArray(), size);
}

// Retail 00AA49D0.
const CCharString C3DAnimFileXSequenceChunk::GetChunkDescription() const
{
    return CCharString("Compressed Animation Sequence");
}

C3DAnimFileXSequenceChunk::C3DAnimFileXSequenceChunk() : CChunkedFileChunk("XSEQ") {}

// Retail 00AA4710: X sequences under objects and movement vectors, and the
// allocation size under the helpers (the returned types are not kept).
C3DAnimFileXLoader::C3DAnimFileXLoader()
{
    RegisterObjectSubChunkType(C3DAnimFileXSequenceChunk());
    RegisterMovementVectorSubChunkType(C3DAnimFileXSequenceChunk());
    RegisterHelpersSubChunkType(C3DAnimFileXAllocationSizeChunk());
}
