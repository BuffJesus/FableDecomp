// lib_3d_mesh_file_engine_material.cpp, retail 0x00AC51D0 / 0x00AC56F0 /
// 0x00AC6180 / 0x00AC6270 (Ego_r C3DMeshFileEngineMaterialChunk): the "EMTL"
// engine material (index, name, map file names and bank indices, flags).
// ReadFromFile 0x00AC5770 and WriteToFile 0x00AC52F0 are not reconstructed
// yet. Supersedes the stand-in GetChunkDescription landing at 0x00AC6270.
#include "fable_chunked_file.h"
#include "fable_data_output_stream.h"

// Retail 00AC51D0.
const CCharString C3DMeshFileEngineMaterialChunk::GetChunkInfo() const
{
    CCharString info = "Name: " + GetName();
    return info;
}

// Retail 00AC56F0.
C3DMeshFileEngineMaterialChunk::C3DMeshFileEngineMaterialChunk()
    : CChunkedFileSuperChunk("EMTL"), MaterialIndex(0), DiffuseMapBankIndex(0), BumpMapBankIndex(0),
      ReflectionMapBankIndex(0), IlluminationMapBankIndex(0), MapFlags(0), SelfIllumination(0),
      TwoSided(false), Transparent(false), BooleanAlpha(false), DegenerateTriangles(false)
{
}

// Retail 00AC6180.
CChunkedFileChunk* C3DMeshFileEngineMaterialChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileEngineMaterialChunk);
}

// Retail 00AC6270.
const CCharString C3DMeshFileEngineMaterialChunk::GetChunkDescription() const
{
    return CCharString("Engine Material");
}

// Retail 00AC5260.
void C3DMeshFileEngineMaterialChunk::CalculateMapFlags()
{
    MapFlags = 0;
    if (!MapFilenames.IsNull())
    {
        if (!MapFilenames->DiffuseMap.IsEmpty())
            MapFlags = 1;
        if (!MapFilenames->BumpMap.IsEmpty())
            MapFlags |= 2;
        if (!MapFilenames->ReflectionMap.IsEmpty())
            MapFlags |= 4;
        if (!MapFilenames->IlluminationMap.IsEmpty())
            MapFlags |= 8;
    }
    else
    {
        if (DiffuseMapBankIndex)
            MapFlags = 1;
        if (BumpMapBankIndex)
            MapFlags |= 2;
        if (ReflectionMapBankIndex)
            MapFlags |= 4;
        if (IlluminationMapBankIndex)
            MapFlags |= 8;
    }
}

// Retail 00AC5190.
C3DMeshFileEngineMaterialChunk::CMapFilenames::CMapFilenames()
{
}

// Retail 00AC52F0: version 4, then the material fields, the map filenames
// behind a flag, and the sub-chunks.
void C3DMeshFileEngineMaterialChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    stream.WriteULONG(4);
    stream.WriteSLONG(MaterialIndex);
    stream.WriteNullTerminatedString(Name);
    stream.WriteULONG(MapFlags);
    stream.WriteULONG(DiffuseMapBankIndex);
    stream.WriteULONG(BumpMapBankIndex);
    stream.WriteULONG(ReflectionMapBankIndex);
    stream.WriteULONG(IlluminationMapBankIndex);
    if (!MapFilenames.IsNull())
    {
        stream.WriteEBOOL(true);
        stream.WriteNullTerminatedString(MapFilenames->DiffuseMap);
        stream.WriteNullTerminatedString(MapFilenames->BumpMap);
        stream.WriteNullTerminatedString(MapFilenames->ReflectionMap);
        stream.WriteNullTerminatedString(MapFilenames->IlluminationMap);
    }
    else
    {
        stream.WriteEBOOL(false);
    }
    stream.WriteULONG(SelfIllumination);
    stream.WriteEBOOL(TwoSided);
    stream.WriteEBOOL(Transparent);
    stream.WriteEBOOL(BooleanAlpha);
    stream.WriteEBOOL(DegenerateTriangles);
    CChunkedFileSuperChunk::WriteToFile(stream, saver);
}

// Retail 00AC5740.
void C3DMeshFileEngineMaterialChunk::RemoveMapFilenames()
{
    MapFilenames.Reset();
}
