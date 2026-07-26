#include "candidates/landscape_background_patch_compressed.h"

bool CLandscapeBackgroundPatch::LoadCompressed(
    CMemoryDataInputStream& stream,
    float worldX,
    float worldY)
{
    unsigned long decompressedSize;
    long nextPosition = stream.ReadPosition + 4;
    if (nextPosition >= 0 && nextPosition < 0x7FFFFFFF)
    {
        if (stream.RemainingInSourceChunk >= 4)
        {
            decompressedSize = *reinterpret_cast<unsigned long*>(stream.Cursor);
            stream.Cursor += 4;
            stream.RemainingInSourceChunk -= 4;
            stream.ReadPosition = nextPosition;
        }
        else
        {
            CMemoryDataInputStream::ReadWithSrcChunkOverflow(
                &stream, reinterpret_cast<unsigned char*>(&decompressedSize), 4);
        }
    }

    unsigned long compressedSize;
    nextPosition = stream.ReadPosition + 4;
    if (nextPosition >= 0 && nextPosition < 0x7FFFFFFF)
    {
        if (stream.RemainingInSourceChunk >= 4)
        {
            compressedSize = *reinterpret_cast<unsigned long*>(stream.Cursor);
            stream.Cursor += 4;
            stream.RemainingInSourceChunk -= 4;
            stream.ReadPosition = nextPosition;
        }
        else
        {
            CMemoryDataInputStream::ReadWithSrcChunkOverflow(
                &stream, reinterpret_cast<unsigned char*>(&compressedSize), 4);
        }
    }

    void* memoryManager = *reinterpret_cast<void**>(DAT_01436ea8 + 0x6B0);
    void* memoryPool = *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(memoryManager) + 8);
    unsigned long outputSize = decompressedSize + 3;
    CTemporaryBuffer output(outputSize, memoryPool, &DAT_013cb498);

    fable_i32 compressedSource = 0;
    if (compressedSize != 0)
    {
        long position = stream.GetPosition();
        long endPosition = position + compressedSize;
        if (endPosition >= 0 && endPosition < 0x7FFFFFFF)
        {
            compressedSource = stream.SourceChunkBase + position;
            stream.SetPositionAssumeWithinSourceChunk(endPosition);
        }
    }

    LZSS_Decompress(
        compressedSource,
        compressedSize,
        output.Data,
        &outputSize,
        0);

    CMemoryDataInputStream decompressedStream(output.Data, decompressedSize);
    return Load(decompressedStream, worldX, worldY);
}
