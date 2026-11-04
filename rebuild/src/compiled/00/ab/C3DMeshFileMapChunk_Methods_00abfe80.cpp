// lib_3d_mesh_file_material.cpp, retail 0x00ABFE80 / 0x00ABFF70 / 0x00ABFFB0
// / 0x00AC03F0 / 0x00AC1290 (Ego_r C3DMeshFileMapChunk): the "MMAP" texture
// map chunk (map type, description and file name ahead of its sub-chunks).
// Supersedes the stand-in GetChunkDescription landing at 0x00AC1290.
#include "fable_chunked_file.h"
#include "fable_counted_pointer_inline.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"

// Retail 00ABFE80.
const CCharString C3DMeshFileMapChunk::GetChunkInfo() const
{
    CCharString info = "Description: " + GetMapDescription() + ", Filename: " + GetFilename();
    return info;
}

// Retail 00ABFF70.
void C3DMeshFileMapChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    stream.WriteSLONG(MapType);
    stream.WriteNullTerminatedString(MapDescription);
    stream.WriteNullTerminatedString(Filename);
    CChunkedFileSuperChunk::WriteToFile(stream, saver);
}

// Retail 00ABFFB0.
CChunkedFileChunk* C3DMeshFileMapChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileMapChunk);
}

// Retail 00AC03F0.
void C3DMeshFileMapChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    const unsigned long start = stream.GetPosition();
    MapType = (EMaterialMapType)stream.ReadValue<long>();
    MapDescription = stream.ReadNullTerminatedString();
    Filename = stream.ReadNullTerminatedString();
    const unsigned long bytes_read = stream.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(stream, size - bytes_read, loader);
}

// Retail 00AC1290.
const CCharString C3DMeshFileMapChunk::GetChunkDescription() const
{
    return CCharString("Texture Map");
}

// Retail 00ABFE20.
C3DMeshFileMapChunk::C3DMeshFileMapChunk(EMaterialMapType type) : CChunkedFileSuperChunk("MMAP"), MapType(type) {}

// Retail 00ABFE50.
C3DMeshFileMapChunk::C3DMeshFileMapChunk() : CChunkedFileSuperChunk("MMAP"), MapType(MATERIAL_MAP_TYPE_NONE) {}

// Retail 00ABFF20.
void C3DMeshFileMapChunk::SetMapDescription(const CCharString& description)
{
    MapDescription = description;
}

// Retail 00ABFF30.
void C3DMeshFileMapChunk::SetFilename(const CCharString& filename)
{
    Filename = filename;
}

// Retail 00ABFF40.
const unsigned long C3DMeshFileMapChunk::GetCRC() const
{
    return CRC;
}

// Retail 00ABFF50.
void C3DMeshFileMapChunk::SetCRC(unsigned long crc)
{
    CRC = crc;
}

// Retail 00ABFF60.
void C3DMeshFileMapChunk::SetType(EMaterialMapType type)
{
    MapType = type;
}

// Retail 00AC0C00 (C3DMeshFileMaterialChunk; here so the map constructor
// expands into it): a new map of the type, added as a sub-chunk.
CCountedPointer<C3DMeshFileMapChunk> C3DMeshFileMaterialChunk::CreateMap(EMaterialMapType type)
{
    CCountedPointer<C3DMeshFileMapChunk> map(new C3DMeshFileMapChunk(type));
    AddSubChunk(CCountedPointer<CChunkedFileChunk>(map));
    return map;
}
