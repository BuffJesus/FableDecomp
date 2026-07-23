#include "candidates/local_detail_cache_compiled.h"

struct StaticMapChunkHeader
{
    fable_u32 ChunkType;
    fable_u32 ChunkSize;
    fable_u32 ChunkFlags;
};

void CLocalDetailCacheMap::OpenStaticMap(
    CEngineStaticMapFileBlockManager* manager,
    CDataInputStream& stream)
{
    StaticMapFileBlockManager = manager;
    StaticMapChunkHeader header;
    header.ChunkSize = 0;
    header.ChunkType = 0;
    reinterpret_cast<unsigned char*>(&header)[8] = 0;

    if ((unsigned long)(stream.ReadPosition + 0x0C) < 0x80000000U)
    {
        if (stream.SrcBytesRemaining < 0x0C)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                &stream,
                reinterpret_cast<unsigned char*>(&header),
                0x0C);
        }
        else
        {
            fable_u32* source = stream.SrcCursor32;
            header.ChunkType = source[0];
            header.ChunkSize = source[1];
            header.ChunkFlags = source[2];
            stream.SrcCursor32 = source + 3;
            stream.SrcBytesRemaining -= 0x0C;
            stream.ReadPosition += 0x0C;
        }
    }

    stream.SetSrcChunk(header.ChunkType);
    QuadTreeElement.LoadHeader(stream);
    ObjectTypeCollectionPalette.Load(stream);
}
