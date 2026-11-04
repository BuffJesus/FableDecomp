// Ego_r lib_mirrored_texture_highlights_chunk.cpp, retail 0x00AC4EC0-0x00AC5080:
// the "MTEH" chunk's vertex-index list (a count, then one ULONG per index).
#include <stdio.h>
#include "fable_chunked_file.h"
#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include <new>
#include "fable_stlport_vector_impl.h"

// Retail 00AC4EC0.
const CCharString CMirroredTextureHighlightsChunk::GetChunkInfo() const
{
    char text[128];
    sprintf(text, "Number of verticies: %u", GetCount());
    return CCharString(text);
}

// Retail 00AC4F20.
unsigned long CMirroredTextureHighlightsChunk::GetCount() const
{
    return TriangleVertexIndexList.size();
}

// Retail 00AC4F30.
const unsigned long* CMirroredTextureHighlightsChunk::GetList() const
{
    return TriangleVertexIndexList.GetAsCArray();
}

// Retail 00AC4F50.
void CMirroredTextureHighlightsChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    const unsigned long count = GetCount();
    stream.WriteULONG(count);
    for (unsigned long i = 0; i < count; ++i)
        stream.WriteULONG(TriangleVertexIndexList[i]);
}

// Retail 00AC4F90.
unsigned long* CMirroredTextureHighlightsChunk::GetList()
{
    return TriangleVertexIndexList.GetAsCArray();
}

// Retail 00AC4FB0.
void CMirroredTextureHighlightsChunk::ClearList()
{
    TriangleVertexIndexList.clear();
}

// Retail 00AC4FE0.
CChunkedFileChunk* CMirroredTextureHighlightsChunk::CreateNewInstance() const
{
    return InitNewInstance(new CMirroredTextureHighlightsChunk);
}

// Retail 00AC5040.
void CMirroredTextureHighlightsChunk::AddVertexIndex(const unsigned long& vertIndex)
{
    TriangleVertexIndexList.push_back(vertIndex);
}

// Retail 00AC5080.
void CMirroredTextureHighlightsChunk::ReadFromFile(CDataInputStream& stream, unsigned long, CChunkedFileLoader&)
{
    const unsigned long count = stream.ReadULONG();
    TriangleVertexIndexList.resize(count);
    for (unsigned long i = 0; i < count; ++i)
        TriangleVertexIndexList[i] = stream.ReadULONG();
}
