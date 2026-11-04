// CDataInputStream::ReadWithSrcChunkOverflow(void*, long) @ 0x00993CA0 (Ego_r
// ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z): the slow path of the
// inline Read, for a read the current source window cannot satisfy. It drains
// what is left of the window; then a stream that wants buffered reads for this
// size refills the window chunk by chunk and copies from it, while any other
// read goes straight to the source and leaves the stream without a window.
#include "fable_data_input_stream.h"

void CDataInputStream::ReadWithSrcChunkOverflow(void* data, long size)
{
    if (SourceChunkBytesRemaining > 0)
    {
        long available = SourceChunkBytesRemaining;
        void* nextData = static_cast<fable_u8*>(data) + available;
        long nextSize = size - available;
        Read(data, available);
        data = nextData;
        size = nextSize;
    }

    if (NeedsBufferedRead(size))
    {
        while (size > 0)
        {
            SourceChunkStreamPos = StreamPos;
            InternalReadNextSourceChunk(CurrentSourcePtr, SourceChunkBytesRemaining);
            long chunk = size < SourceChunkBytesRemaining ? size : SourceChunkBytesRemaining;
            Read(data, chunk);
            data = static_cast<fable_u8*>(data) + chunk;
            size -= chunk;
        }
    }
    else
    {
        unsigned long position = StreamPos;
        InternalReadUnbuffered(static_cast<fable_u8*>(data), size);
        StreamPos = position + size;
        SourceChunkBytesRemaining = 0;
        CurrentSourcePtr = 0;
        SourceChunkStreamPos = 0;
    }
}
