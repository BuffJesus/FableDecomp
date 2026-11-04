// DataInputStream unit: CDataInputStream::Close() @ 0x00993C80 and
// CFileDataInputStream::Close() @ 0x00994300 (FableWin ?Close@CDataInputStream@@UAEXXZ,
// ?Close@CFileDataInputStream@@UAEXXZ; vtable slot 3).
//
// Closing a stream forgets its position, size and source window. A file stream
// also frees its buffer and, while its file is still open, leaves the file at
// the stream's position before letting go of it. Retail expands the base Close
// into the file stream's (/O2 expands a body defined earlier in the unit), so
// both are defined here. <new> declares operator delete[], which retail calls
// (without it VC7.1 calls operator delete).
#include <new>
#include "fable_data_input_stream.h"
#include "fable_filesystem.h"

void CDataInputStream::Close()
{
    StreamPos = 0;
    StreamSize = 0;
    SourceChunkStreamPos = 0;
    SourceChunkBytesRemaining = 0;
    CurrentSourcePtr = 0;
}

void CFileDataInputStream::Close()
{
    if (Buffer)
    {
        delete[] Buffer;
        Buffer = 0;
        BufferSize = 0;
    }
    if (File && File->IsOpen())
        File->SetPosition(GetPosition());
    File = 0;
    CDataInputStream::Close();
}
