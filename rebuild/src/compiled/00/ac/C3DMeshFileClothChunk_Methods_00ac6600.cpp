// lib_3d_mesh_file_cloth.cpp, retail 0x00AC6600 / 0x00AC66E0 / 0x00AC6750 (Ego_r
// C3DMeshFileClothChunk): the "CLTH" cloth description (name, material,
// particle code and, when compiled, its byte code) ahead of its sub-chunks.
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include "fable_stlport_vector_impl.h"

// Retail 00AC6600: format version 2.
void C3DMeshFileClothChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    stream.WriteULONG(2);
    stream.WriteNullTerminatedString(Name);
    stream.WriteULONG(MaterialIndex);
    stream.WriteNullTerminatedString(ParticleCode);
    const bool has_compiled_code = !CompiledCode.empty();
    stream.WriteEBOOL(has_compiled_code);
    if (has_compiled_code)
    {
        stream.WriteULONG(CompiledCode.size());
        stream.Write(CompiledCode.GetAsCArray(), CompiledCode.size());
    }
    CChunkedFileSuperChunk::WriteToFile(stream, saver);
}

// Retail 00AC66E0.
CChunkedFileChunk* C3DMeshFileClothChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileClothChunk);
}

// Retail 00AC6750: the version word is not used.
void C3DMeshFileClothChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    const unsigned long start = stream.GetPosition();
    stream.ReadULONG();
    Name = stream.ReadNullTerminatedString();
    MaterialIndex = stream.ReadULONG();
    ParticleCode = stream.ReadNullTerminatedString();
    if (stream.ReadEBOOL())
    {
        const unsigned long byte_count = stream.ReadULONG();
        CompiledCode.resize(byte_count, 0);
        stream.Read(CompiledCode.GetAsCArray(), byte_count);
    }
    const unsigned long bytes_read = stream.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(stream, size - bytes_read, loader);
}

C3DMeshFileClothChunk::C3DMeshFileClothChunk() : CChunkedFileSuperChunk("CLTH") {}

// Retail 00AC62B0.
const CCharString C3DMeshFileClothChunk::GetChunkInfo() const
{
    return CCharString("");
}

// Retail 00AC62D0.
const CCharString C3DMeshFileClothChunk::GetName() const
{
    return Name;
}

// Retail 00AC62F0.
void C3DMeshFileClothChunk::SetName(const CCharString& name)
{
    Name = name;
}

// Retail 00AC6300.
void C3DMeshFileClothChunk::SetParticleCode(const CCharString& code)
{
    ParticleCode = code;
}

// Retail 00AC6310.
void C3DMeshFileClothChunk::SetMaterialIndex(long index)
{
    MaterialIndex = index;
}
