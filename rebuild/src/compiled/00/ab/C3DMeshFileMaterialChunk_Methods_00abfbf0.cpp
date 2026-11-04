// lib_3d_mesh_file_material.cpp, retail 0x00ABFBF0 / 0x00ABFCF0 / 0x00AC0250
// / 0x00AC0FC0 / 0x00AC1E70 (Ego_r C3DMeshFileMaterialChunk): the "MTRL"
// material (index, sidedness, colours, shininess, transparency and name)
// ahead of its map and extended-property sub-chunks. Supersedes the stand-in GetChunkDescription
// landing at 0x00AC1E70.
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"

// Retail 00ABFBF0.
const CCharString C3DMeshFileMaterialChunk::GetChunkInfo() const
{
    CCharString info = "Name: " + GetName();
    return info;
}

// Retail 00ABFCF0.
void C3DMeshFileMaterialChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    stream.WriteNullTerminatedString(Name);
    stream.WriteSLONG(MaterialIndex);
    stream.WriteEBOOL(TwoSided);
    stream.WriteColour(Ambient);
    stream.WriteColour(Diffuse);
    stream.WriteColour(Specular);
    stream.WriteFloat(Shiny);
    stream.WriteFloat(ShinyStrength);
    stream.WriteFloat(Transparency);
    CChunkedFileSuperChunk::WriteToFile(stream, saver);
}

// Retail 00AC0010.
void C3DMeshFileMaterialChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    const unsigned long start = stream.GetPosition();
    Name = stream.ReadNullTerminatedString();
    MaterialIndex = stream.ReadValue<long>();
    TwoSided = stream.ReadEBOOL();
    Ambient = stream.ReadColour();
    Diffuse = stream.ReadColour();
    Specular = stream.ReadColour();
    Shiny = stream.ReadFloat();
    ShinyStrength = stream.ReadFloat();
    Transparency = stream.ReadFloat();
    const unsigned long bytes_read = stream.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(stream, size - bytes_read, loader);
}

// Retail 00AC0250: opaque black colours, one-sided, matte, opaque.
void C3DMeshFileMaterialChunk::ResetParams()
{
    TwoSided = false;
    Ambient = CRGBColour(0, 0, 0);
    Diffuse = CRGBColour(0, 0, 0);
    Specular = CRGBColour(0, 0, 0);
    Shiny = 0.0f;
    ShinyStrength = 0.0f;
    Transparency = 0.0f;
}

// Retail 00AC0FC0.
CChunkedFileChunk* C3DMeshFileMaterialChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileMaterialChunk);
}

// Retail 00AC1E70.
const CCharString C3DMeshFileMaterialChunk::GetChunkDescription() const
{
    return CCharString("Material Description");
}

C3DMeshFileMaterialChunk::C3DMeshFileMaterialChunk() : CChunkedFileSuperChunk("MTRL"), MaterialIndex(-1)
{
    ResetParams();
}

// Retail 00AC0A80.
C3DMeshFileMaterialChunk::C3DMeshFileMaterialChunk(long index) : CChunkedFileSuperChunk("MTRL"), MaterialIndex(index)
{
    ResetParams();
}

// Retail 00ABFC40.
void C3DMeshFileMaterialChunk::SetMaterialIndex(long index)
{
    MaterialIndex = index;
}

// Retail 00ABFC50.
void C3DMeshFileMaterialChunk::SetName(const CCharString& name)
{
    Name = name;
}

// Retail 00ABFC60.
void C3DMeshFileMaterialChunk::SetTwoSided(bool two_sided)
{
    TwoSided = two_sided;
}

// Retail 00ABFC70.
void C3DMeshFileMaterialChunk::SetAmbient(CRGBColour colour)
{
    Ambient = colour;
}

// Retail 00ABFC80.
void C3DMeshFileMaterialChunk::SetDiffuse(CRGBColour colour)
{
    Diffuse = colour;
}

// Retail 00ABFC90.
void C3DMeshFileMaterialChunk::SetSpecular(CRGBColour colour)
{
    Specular = colour;
}

// Retail 00ABFCA0.
void C3DMeshFileMaterialChunk::SetShiny(float shiny)
{
    Shiny = shiny;
}

// Retail 00ABFCB0.
void C3DMeshFileMaterialChunk::SetShinyStrength(float strength)
{
    ShinyStrength = strength;
}

// Retail 00ABFCC0.
void C3DMeshFileMaterialChunk::SetTransparency(float transparency)
{
    Transparency = transparency;
}
