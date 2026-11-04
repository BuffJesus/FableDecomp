// Behaviour fixture for the DataInputStream unit's Close pair:
// CDataInputStream::Close 0x00993C80 and CFileDataInputStream::Close 0x00994300.
#include <stdio.h>
#include <string.h>
#include <new>

#include "fable_data_input_stream.h"
#include "fable_filesystem.h"
#include "test_support/base_class_stubs.h"
#define FABLE_STUBS_NO_DATA_INPUT_STREAM_CLOSE
#define FABLE_STUBS_NO_FILE_DATA_INPUT_STREAM_CLOSE
#include "test_support/data_input_stream_stubs.h"

class CFileInformation
{
};

namespace
{
    int g_deletes;
    void* g_deleted;
    int g_setPositions;
    long g_setPositionValue;
    bool g_open;
}

void operator delete[](void* p)
{
    ++g_deletes;
    g_deleted = p;
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
    virtual void Read(void*, long, CASuspendableProcess*) {}
    virtual void Write(const void*, long, CASuspendableProcess*) {}
    virtual void SetPosition(long position) { ++g_setPositions; g_setPositionValue = position; }
    virtual void MovePosition(long) {}
    virtual long GetPosition() const { return 0; }
    virtual void SetLength(long) {}
    virtual long GetLength() const { return 0; }
    virtual bool IsOpen() const { return g_open; }
    virtual CWideString GetPathname() const { return CWideString(); }
    virtual CAFile* Clone() const { return 0; }
};

namespace
{
    void Fill(CDataInputStream* s)
    {
        s->StreamPos = 21;
        s->StreamSize = 99;
        s->CurrentSourcePtr = (fable_u8*)0x1000;
        s->SourceChunkStreamPos = 20;
        s->SourceChunkBytesRemaining = 5;
    }

    bool Cleared(const CDataInputStream* s)
    {
        return s->StreamPos == 0 && s->StreamSize == 0 && s->CurrentSourcePtr == 0 &&
               s->SourceChunkStreamPos == 0 && s->SourceChunkBytesRemaining == 0;
    }
}

int main()
{
    int fail = 0;
    FakeFile file;
    fable_u8 buffer[16];

    // The base Close clears the stream state only.
    CFileDataInputStream a(&file, 16);
    Fill(&a);
    a.CDataInputStream::Close();
    if (!Cleared(&a) || a.File != &file || a.BufferSize != 16) fail |= 1;

    // A buffered stream on an open file: the buffer is freed and the file left
    // at the stream's position.
    CFileDataInputStream b(&file, 16);
    Fill(&b);
    b.Buffer = buffer;
    g_open = true; g_deletes = 0; g_setPositions = 0;
    b.Close();
    if (!Cleared(&b) || b.File != 0 || b.Buffer != 0 || b.BufferSize != 0 || g_deletes != 1 ||
        g_deleted != buffer || g_setPositions != 1 || g_setPositionValue != 21)
        fail |= 2;

    // No buffer, a closed file: nothing freed, the file untouched.
    CFileDataInputStream c(&file, 8);
    Fill(&c);
    g_open = false; g_deletes = 0; g_setPositions = 0;
    c.Close();
    if (!Cleared(&c) || c.File != 0 || c.BufferSize != 8 || g_deletes != 0 || g_setPositions != 0) fail |= 4;

    // No file at all.
    CFileDataInputStream d(0, 8);
    Fill(&d);
    d.Close();
    if (!Cleared(&d) || g_setPositions != 0) fail |= 8;

    if (fail == 0)
        printf("FABLETLC_DATAINPUTSTREAM_CLOSE_00994300_PASS\n");
    else
        printf("FABLETLC_DATAINPUTSTREAM_CLOSE_00994300 FAIL %d\n", fail);
    return fail;
}
