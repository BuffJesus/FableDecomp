// lib_data_stream.cpp, retail 0x009944A0-0x00994870: CBufferedFileDataOutputStream
// (Ego_r names). Writes land in a buffer mirroring [DataStartFilePtr,
// DataStartFilePtr + BufferDataSize) of the file; Flush writes it back and
// restores the file position, Close and the destructor flush and free it.
// Flush is defined first, so Close and the destructor expand it.
#include <new>
#include <string.h>
#include "fable_data_output_stream.h"
#include "fable_filesystem.h"

// Retail 00994520.
void CBufferedFileDataOutputStream::Flush()
{
    if (BufferDataSize > 0)
    {
        File->SetPosition(DataStartFilePtr);
        File->Write(Buffer, BufferDataSize, 0);
        File->SetPosition(FileSize);
        BufferDataSize = 0;
    }
    DataStartFilePtr = FilePos;
}

// Retail 00994810.
void CBufferedFileDataOutputStream::Close()
{
    Flush();
    delete[] Buffer;
    Buffer = 0;
}

// Retail 009944A0: buffer from the file's current position.
CBufferedFileDataOutputStream::CBufferedFileDataOutputStream(CAFile& file, long bufferSize)
{
    File = &file;
    BufferSize = bufferSize;
    Buffer = new unsigned char[bufferSize];
    BufferDataSize = 0;
    DataStartFilePtr = 0;
    FilePos = FileSize = file.GetPosition();
}

// Retail 009947A0.
CBufferedFileDataOutputStream::~CBufferedFileDataOutputStream()
{
    Flush();
    delete[] Buffer;
}

// Retail 00994500.
void CBufferedFileDataOutputStream::SetPosition(unsigned long position)
{
    FilePos = position;
}

// Retail 00994510.
unsigned long CBufferedFileDataOutputStream::GetPosition() const
{
    return FilePos;
}

// Retail 00994870: a write larger than the buffer goes straight to the file.
void CBufferedFileDataOutputStream::Write(const void* data, long size)
{
    if (size > BufferSize)
    {
        Flush();
        File->SetPosition(FilePos);
        File->Write(data, size, 0);
        FilePos += size;
        if (FileSize < FilePos)
            FileSize = FilePos;
        File->SetPosition(FileSize);
        return;
    }
    long offset = FilePos - DataStartFilePtr;
    if (FilePos < DataStartFilePtr || offset + size > BufferSize || offset > BufferDataSize)
    {
        Flush();
        offset = FilePos - DataStartFilePtr;
    }
    memcpy(Buffer + offset, data, size);
    FilePos += size;
    if (BufferDataSize < offset + size)
        BufferDataSize = offset + size;
    if (FileSize < FilePos)
        FileSize = FilePos;
}
