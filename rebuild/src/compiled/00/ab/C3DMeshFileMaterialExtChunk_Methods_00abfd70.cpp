// lib_3d_mesh_file_material.cpp, retail 0x00ABFD70 / 0x00ABFDE0 / 0x00AC03B0
// / 0x00AC1920 (Ego_r C3DMeshFileMaterialExtChunk): the "MTLE" extended
// material properties (a version word, then the self-illumination level).
// ReadFromFile 0x00AC02D0 is a near match left out (see
// C3DMeshFileXLeafChunks_00aa4af0.cpp). Supersedes the stand-in landings at
// 0x00ABFDE0 and 0x00AC1920.
#include "fable_chunked_file.h"
#include "fable_data_output_stream.h"

// Retail 00ABFD70.
const CCharString C3DMeshFileMaterialExtChunk::GetChunkInfo() const
{
    return CCharString("Self Illumination: ") + GFIntToCharString(SelfIllumination);
}

// Retail 00ABFDE0.
void C3DMeshFileMaterialExtChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    stream.WriteULONG(1);
    stream.WriteULONG(SelfIllumination);
}

// Retail 00AC03B0.
CChunkedFileChunk* C3DMeshFileMaterialExtChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileMaterialExtChunk);
}

// Retail 00AC1920.
const CCharString C3DMeshFileMaterialExtChunk::GetChunkDescription() const
{
    return CCharString("Extended Material Properties");
}

C3DMeshFileMaterialExtChunk::C3DMeshFileMaterialExtChunk() : CChunkedFileChunk("MTLE") {}

// Retail 00ABFDC0.
unsigned long C3DMeshFileMaterialExtChunk::GetSelfIllumination() const
{
    return SelfIllumination;
}

// Retail 00ABFDD0.
void C3DMeshFileMaterialExtChunk::SetSelfIllumination(unsigned long value)
{
    SelfIllumination = value;
}

// Retail 00ABFE10: nothing to reset in this build.
void C3DMeshFileMaterialExtChunk::ResetParams()
{
}
