// CFileDataInputStream::InternalReadNextSourceChunk(const fable_u8*&, long&) @ 0x00994360
// (Ego_r ?InternalReadNextSourceChunk@CFileDataInputStream@@EAEXAAPBEAAJ@Z, vtable
// 0x129A728 slot 8): refills the buffer from the file at the stream position,
// at most a buffer's worth and no further than the end of the stream.
#include "fable_data_input_stream.h"
#include "fable_filesystem.h"

void CFileDataInputStream::InternalReadNextSourceChunk(const fable_u8*& source, long& size)
{
    unsigned long position = GetPosition();
    unsigned long left = StreamSize - position;
    size = (unsigned long)BufferSize < left ? BufferSize : left;
    File->SetPosition(position);
    File->Read(Buffer, size, 0);
    source = Buffer;
}