// lib_data_stream.cpp, retail 0x00993C10-0x00993C60: moving within the
// current source chunk, and opening a stream over memory (Ego_r names).
#include "fable_data_input_stream.h"

// Retail 00993C10: the caller has checked the position is inside the chunk.
void CDataInputStream::SetPositionAssumeWithinSourceChunk(unsigned long position)
{
    CurrentSourcePtr += position - StreamPos;
    SourceChunkBytesRemaining += StreamPos - position;
    StreamPos = position;
}

// Retail 00993C40.
bool CDataInputStream::CanSeekToPosWithinSourceChunk(long position)
{
    return position >= SourceChunkStreamPos && position <= StreamPos + SourceChunkBytesRemaining;
}

// Retail 00993C60: positioned at 0 with `chunkBytes` readable from `data`.
void CDataInputStream::OpenStream(const unsigned char* data, long streamSize, long chunkBytes)
{
    StreamPos = 0;
    SourceChunkStreamPos = 0;
    CurrentSourcePtr = const_cast<unsigned char*>(data);
    StreamSize = streamSize;
    SourceChunkBytesRemaining = chunkBytes;
}
