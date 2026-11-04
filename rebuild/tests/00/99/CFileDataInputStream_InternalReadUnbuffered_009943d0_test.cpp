// Behaviour fixture for CFileDataInputStream::InternalReadUnbuffered (0x009943D0).
#include <stdio.h>
#include <string.h>

#include "fable_data_input_stream.h"
#include "fable_filesystem.h"
#include "test_support/base_class_stubs.h"
#define FABLE_STUBS_NO_FILE_READ_UNBUFFERED
#include "test_support/data_input_stream_stubs.h"

class CFileInformation
{
};

namespace
{
    int g_setPositions, g_reads;
    long g_setPositionValue;
    void* g_readData;
    long g_readSize;
    CASuspendableProcess* g_readProcess;
}

CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CDataInputStream::CDataInputStream()
    : StreamPos(0), StreamSize(0), CurrentSourcePtr(0), SourceChunkStreamPos(0), SourceChunkBytesRemaining(0)
{
}
void CDataInputStream::SetPosition(unsigned long position) { StreamPos = position; }
CFileDataInputStream::CFileDataInputStream(CAFile* file, unsigned long bufferSize)
{
    File = file;
    Buffer = 0;
    BufferSize = (long)bufferSize;
}
CFileDataInputStream::~CFileDataInputStream() {}

CAFile::CAFile() {}

CFileInformation CAFile::GetFileInformation() { return CFileInformation(); }
CWideString::CWideString() : storage_(0) {}
CWideString::~CWideString() {}

class FakeFile : public CAFile
{
public:
    virtual void Open(const CWideString&, FileAccessType, unsigned long) {}
    virtual void Close() {}
    virtual void Read(void* data, long size, CASuspendableProcess* process)
    {
        ++g_reads; g_readData = data; g_readSize = size; g_readProcess = process;
    }
    virtual void Write(const void*, long, CASuspendableProcess*) {}
    virtual void SetPosition(long position) { ++g_setPositions; g_setPositionValue = position; }
    virtual void MovePosition(long) {}
    virtual long GetPosition() const { return 0; }
    virtual void SetLength(long) {}
    virtual long GetLength() const { return 0; }
    virtual bool IsOpen() const { return true; }
    virtual CWideString GetPathname() const { return CWideString(); }
    virtual CAFile* Clone() const { return 0; }
};

class CDataInputStreamTestAccess
{
public:
    static void NextChunk(CFileDataInputStream* s, const fable_u8*& source, long& size) { s->InternalReadNextSourceChunk(source, size); }
    static void Unbuffered(CFileDataInputStream* s, fable_u8* data, long size) { s->InternalReadUnbuffered(data, size); }
};

int main()
{
    int fail = 0;
    FakeFile file;
    fable_u8 data[8];

    CFileDataInputStream s(&file, 16);
    s.StreamPos = 33;
    CDataInputStreamTestAccess::Unbuffered(&s, data, 8);
    if (g_setPositions != 1 || g_setPositionValue != 33 || g_reads != 1 || g_readData != data ||
        g_readSize != 8 || g_readProcess != 0 || s.StreamPos != 33)
        fail |= 1;

    if (fail == 0)
        printf("FABLETLC_FILEDATAINPUTSTREAM_READUNBUFFERED_009943D0_PASS\n");
    else
        printf("FABLETLC_FILEDATAINPUTSTREAM_READUNBUFFERED_009943D0 FAIL %d\n", fail);
    return fail;
}