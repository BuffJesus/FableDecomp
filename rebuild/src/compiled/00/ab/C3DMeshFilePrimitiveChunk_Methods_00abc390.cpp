// lib_3d_mesh_file_primitive.cpp, retail 0x00ABC390, 0x00ABCF40, 0x00ABD0D0
// and 0x00ABEE90 (Ego_r C3DMeshFilePrimitiveChunk): the "PRIM" chunk naming
// the primitive's material indices. A single index is stored as itself; a
// list is stored as its count with the top bit set, then the indices.
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"

// Retail 00ABC390.
void C3DMeshFilePrimitiveChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    if (MaterialIndices.size() == 1)
    {
        stream.WriteSLONG(MaterialIndices[0]);
    }
    else
    {
        const long count = MaterialIndices.size();
        stream.WriteSLONG(count | 0x80000000);
        for (long i = 0; i < count; ++i)
            stream.WriteSLONG(MaterialIndices[i]);
    }
    CChunkedFileSuperChunk::WriteToFile(stream, saver);
}

// Retail 00ABCF40.
void C3DMeshFilePrimitiveChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    const unsigned long start = stream.GetPosition();
    const long value = stream.ReadValue<long>();
    if (value != -1 && (value & 0x80000000))
    {
        const long count = value & 0x7FFFFFFF;
        MaterialIndices.resize(count);
        for (long i = 0; i < count; ++i)
            MaterialIndices[i] = stream.ReadValue<long>();
    }
    else
    {
        SetMaterialIndex(value);
    }
    const unsigned long bytes_read = stream.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(stream, size - bytes_read, loader);
}

// Retail 00ABD0D0.
CChunkedFileChunk* C3DMeshFilePrimitiveChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFilePrimitiveChunk);
}

// Retail 00ABEE90.
const CCharString C3DMeshFilePrimitiveChunk::GetChunkDescription() const
{
    return CCharString("Primitive");
}

C3DMeshFilePrimitiveChunk::C3DMeshFilePrimitiveChunk()
    : CChunkedFileSuperChunk("PRIM")
{
    SetMaterialIndex(-1);
}
