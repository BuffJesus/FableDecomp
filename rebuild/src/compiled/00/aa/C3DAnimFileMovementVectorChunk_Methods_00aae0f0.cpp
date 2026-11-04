// lib_3d_anim_file_movement_vector.cpp, retail 0x00AAE0F0 / 0x00AAE130 /
// 0x00AAE2C0 / 0x00AAE400 / 0x00AAEE90 (Ego_r C3DAnimFileMovementVectorChunk):
// the "MVEC" chunk carrying a sequence's root movement vector ahead of its
// sub-chunks. Supersedes the stand-in landings at 0x00AAE0F0 and 0x00AAEE90.
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"

// Retail 00AAE0F0.
const CCharString C3DAnimFileMovementVectorChunk::GetChunkInfo() const
{
    return CCharString("");
}

// Retail 00AAE130.
void C3DAnimFileMovementVectorChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    stream.WriteFloat(Vector.X);
    stream.WriteFloat(Vector.Y);
    stream.WriteFloat(Vector.Z);
    CChunkedFileSuperChunk::WriteToFile(stream, saver);
}

// Retail 00AAE2C0.
void C3DAnimFileMovementVectorChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    const unsigned long end = stream.GetPosition() + size;
    Vector.X = stream.ReadFloat();
    Vector.Y = stream.ReadFloat();
    Vector.Z = stream.ReadFloat();
    CChunkedFileSuperChunk::ReadFromFile(stream, end - stream.GetPosition(), loader);
}

// Retail 00AAE400.
CChunkedFileChunk* C3DAnimFileMovementVectorChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DAnimFileMovementVectorChunk);
}

// Retail 00AAEE90.
const CCharString C3DAnimFileMovementVectorChunk::GetChunkDescription() const
{
    return CCharString("Movement Vector");
}

C3DAnimFileMovementVectorChunk::C3DAnimFileMovementVectorChunk() : CChunkedFileSuperChunk("MVEC") {}

void C3DAnimFileMovementVectorChunk::SetVector(const C3DVector& vector)
{
    Vector = vector;
}
