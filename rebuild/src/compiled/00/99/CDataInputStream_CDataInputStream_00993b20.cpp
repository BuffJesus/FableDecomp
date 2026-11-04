// DataInputStream unit: CDataInputStream::CDataInputStream() @ 0x00993B20 and
// CFileDataInputStream::CFileDataInputStream(CAFile*, unsigned long) @ 0x00994700
// (FableWin ??0CDataInputStream@@QAE@XZ, ??0CFileDataInputStream@@QAE@PAVCAFile@@K@Z).
//
// The base stream starts empty. The file stream wraps `file`; with a buffer
// size it allocates the read window and places the stream at the file's
// current position over the file's length. Retail expands the base
// constructor into the file stream's (/O2 expands a body defined earlier in
// the unit), so both are defined here. <new> is included for retail's array
// allocation through operator new[] (without its declaration VC7.1 calls
// operator new).
#include <new>
#include "fable_data_input_stream.h"
#include "fable_string.h"
#include "fable_filesystem.h"
#include "fable_data_output_stream.h"

CDataInputStream::CDataInputStream()
    : StreamPos(0),
      StreamSize(0),
      CurrentSourcePtr(0),
      SourceChunkStreamPos(0),
      SourceChunkBytesRemaining(0)
{
}

inline CDataInputStream::CDataInputStream(const unsigned char* data, long streamSize, long chunkBytes)
    : StreamPos(0),
      StreamSize(streamSize),
      CurrentSourcePtr(const_cast<unsigned char*>(data)),
      SourceChunkStreamPos(0),
      SourceChunkBytesRemaining(chunkBytes)
{
}

// Open an unowned memory range, resetting both stream positions. Retail 0x00994570.
void CMemoryDataInputStream::OpenMemorySource(const void* data, unsigned long size)
{
    StreamPos = 0;
    SourceChunkStreamPos = 0;
    CurrentSourcePtr = (unsigned char*)data;
    StreamSize = size;
    SourceChunkBytesRemaining = size;
    StreamSource = (unsigned char*)data;
}

CFileDataInputStream::CFileDataInputStream(CAFile* file, unsigned long bufferSize)
    : File(file)
{
    if (bufferSize > 0)
    {
        Buffer = new fable_u8[bufferSize];
        BufferSize = bufferSize;
        long length = file->GetLength();
        CurrentSourcePtr = Buffer;
        StreamPos = 0;
        SourceChunkStreamPos = 0;
        StreamSize = length;
        SourceChunkBytesRemaining = 0;
        SetPosition(file->GetPosition());
    }
}

// Retail 0x00994930: a stream over [data, data + size).
CMemoryDataInputStream::CMemoryDataInputStream(const void* data, unsigned long size)
    : CDataInputStream(static_cast<const unsigned char*>(data), size, size),
      StreamSource((unsigned char*)data)
{
}

inline void CMemoryDataInputStream::OpenStringSource(const CCharString& source)
{
    OpenMemorySource(source.GetConstCharPtr(), source.GetByteLength());
}

// Retail 0x00995F50: a stream over the string's characters.
CMemoryDataInputStream::CMemoryDataInputStream(const CCharString& source)
{
    OpenStringSource(source);
}

// The output stream owns the bytes; this input stream borrows its full written range.
// These accessors are expanded in the retail DataInputStream translation unit.
unsigned long CMemoryDataOutputStream::GetLength() const { return Data.size(); }
const void* CMemoryDataOutputStream::PeekData() const { return Data.GetAsCArray(); }

CMemoryDataInputStream::CMemoryDataInputStream(const CMemoryDataOutputStream& source)
    : CDataInputStream(static_cast<const unsigned char*>(source.PeekData()), source.GetLength(), source.GetLength()),
      StreamSource((unsigned char*)source.PeekData())
{
}

void CDataOutputStream::Write(const CMemoryDataOutputStream& source)
{
    Write(source.PeekData(), source.GetLength());
}

void CMemoryDataOutputStream::Write(const void* data, long size)
{
    if (size > 0)
    {
        __int64 newPosition = (__int64)CurrentPosition + size;
        if (newPosition >= 0 && newPosition <= 0x7FFFFFFF)
        {
            if (CurrentPosition + size > Data.size())
                Data.resize(CurrentPosition + size);
            memcpy((char*)PeekData() + CurrentPosition, data, size);
            CurrentPosition += size;
        }
    }
}