// Behaviour fixture for the DataInputStream unit's constructors:
// CDataInputStream() 0x00993B20, CMemoryDataInputStream::OpenMemorySource
// 0x00994570, CFileDataInputStream(CAFile*, unsigned long) 0x00994700,
// CMemoryDataInputStream(const void*, unsigned long) 0x00994930 and
// CMemoryDataInputStream(const CCharString&) 0x00995F50.
#include <stdio.h>
#include <string.h>
#include <new>

#define private public
#include "fable_string.h"
#undef private
#include "fable_data_input_stream.h"
#include "fable_filesystem.h"
#include "fable_data_output_stream.h"
#include "test_support/base_class_stubs.h"
#include "test_support/data_input_stream_stubs.h"

class CFileInformation
{
};

namespace
{
    int g_sequence;
    int g_setPositionAt;
    unsigned long g_setPositionValue;
    bool g_windowReadyAtSetPosition;
    unsigned long g_newSize;
    int g_newAt;
    fable_u8 g_buffer[64];
    CFileDataInputStream* g_stream;
}

void* operator new[](size_t size)
{
    g_newSize = (unsigned long)size;
    g_newAt = ++g_sequence;
    return g_buffer;
}
void operator delete[](void*) {}

CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CFileDataInputStream::~CFileDataInputStream() {}
CMemoryDataInputStream::~CMemoryDataInputStream() {}
char g_FableEmptyNarrowString_0129AAF4[1] = { 0 };
// Every read here stays inside the opened range.
void CDataInputStream::ReadWithSrcChunkOverflow(void*, long) { abort(); }

// A memory stream over `data`/`size`, freshly positioned.
static bool OpenOver(const CMemoryDataInputStream* s, const void* data, unsigned long size)
{
    return s->StreamPos == 0 && s->SourceChunkStreamPos == 0 && s->CurrentSourcePtr == data &&
           s->StreamSize == (long)size && s->SourceChunkBytesRemaining == (long)size && s->StreamSource == data;
}

void CDataInputStream::SetPosition(unsigned long position)
{
    g_setPositionAt = ++g_sequence;
    g_setPositionValue = position;
    g_windowReadyAtSetPosition =
        this == g_stream && CurrentSourcePtr == g_buffer && StreamPos == 0 &&
        SourceChunkStreamPos == 0 && SourceChunkBytesRemaining == 0 && StreamSize == 1234;
}

CWideString::CWideString() : storage_(0) {}
CWideString::~CWideString() {}
CAFile::CAFile() {}

CFileInformation CAFile::GetFileInformation() { return CFileInformation(); }

class FakeFile : public CAFile
{
public:
    virtual void Open(const CWideString&, FileAccessType, unsigned long) {}
    virtual void Close() {}
    virtual void Read(void*, long, CASuspendableProcess*) {}
    virtual void Write(const void*, long, CASuspendableProcess*) {}
    virtual void SetPosition(long) {}
    virtual void MovePosition(long) {}
    virtual long GetPosition() const { ++g_sequence; return 77; }
    virtual void SetLength(long) {}
    virtual long GetLength() const { ++g_sequence; return 1234; }
    virtual bool IsOpen() const { return true; }
    virtual CWideString GetPathname() const { return CWideString(); }
    virtual CAFile* Clone() const { return 0; }
};

static const void* g_written;
static long g_writtenSize;
static void* g_writer;
static int g_writeCalls;
static void __fastcall CaptureWrite(void* self, void*, const void* data, long size)
{
    g_writer = self; g_written = data; g_writtenSize = size; ++g_writeCalls;
}

// Behaviour of CMemoryDataOutputStream::Write 0x00996CC0 (and PeekData
// 0x00995E70, inlined into it): PeekData is the array object itself while
// Data is empty and Data's first byte otherwise; Write ignores sizes <= 0 and
// writes that would pass 2 GB, grows Data (through resize) only when the
// bytes run past its end, copies them in at the position and advances it.
#include "fable_data_output_stream.h"
#include <stdio.h>
#include <string.h>

static char g_outputBuffer[64];
static int g_resizeCalls;
static size_t g_resizeTo;

namespace _STL
{
    // Stub: grow into g_outputBuffer, zero filling like STLport's resize(n, char()).
    template <>
    void vector<char>::resize(size_t n, const char& fill)
    {
        ++g_resizeCalls; g_resizeTo = n;
        if (this->_M_start == 0) this->_M_start = g_outputBuffer;
        for (char* p = this->_M_finish ? this->_M_finish : this->_M_start; p < this->_M_start + n; ++p) *p = fill;
        this->_M_finish = this->_M_start + n;
        this->_M_end_of_storage._M_data = g_outputBuffer + sizeof(g_outputBuffer);
    }
}

static int TestOutputWrite()
{
    static unsigned char store[sizeof(CMemoryDataOutputStream)];
    memset(store, 0, sizeof(store));
    CMemoryDataOutputStream* s = reinterpret_cast<CMemoryDataOutputStream*>(store);
    memset(g_outputBuffer, 0x55, sizeof(g_outputBuffer));

    if (s->PeekData() != (const void*)&s->Data) { puts("peek empty"); return 1; }

    s->CMemoryDataOutputStream::Write("xyz", 0);
    s->CMemoryDataOutputStream::Write("xyz", -3);
    if (g_resizeCalls != 0 || s->CurrentPosition != 0) { puts("non-positive size"); return 1; }

    s->CMemoryDataOutputStream::Write("abcd", 4);
    if (g_resizeCalls != 1 || g_resizeTo != 4 || s->CurrentPosition != 4 || memcmp(g_outputBuffer, "abcd", 4) != 0)
    { puts("first write"); return 1; }
    if (s->PeekData() != (const void*)g_outputBuffer) { puts("peek data"); return 1; }

    // Overwrite inside the buffer, ending exactly at its end: no resize.
    s->CurrentPosition = 1;
    s->CMemoryDataOutputStream::Write("XYZ", 3);
    if (g_resizeCalls != 1 || s->CurrentPosition != 4 || memcmp(g_outputBuffer, "aXYZ", 4) != 0)
    { puts("in-place write"); return 1; }

    // Past the end: grows to position + size.
    s->CurrentPosition = 6;
    s->CMemoryDataOutputStream::Write("pq", 2);
    if (g_resizeCalls != 2 || g_resizeTo != 8 || s->CurrentPosition != 8 || memcmp(g_outputBuffer, "aXYZ\0\0pq", 8) != 0)
    { puts("grow write"); return 1; }

    // A write that would pass 2 GB is dropped.
    s->CurrentPosition = 0x7FFFFFF0;
    s->CMemoryDataOutputStream::Write("0123456789abcdef0123", 20);
    if (g_resizeCalls != 2 || s->CurrentPosition != 0x7FFFFFF0) { puts("2 GB limit"); return 1; }

    puts("FABLETLC_CMEMORYDATAOUTPUTSTREAM_WRITE_00996CC0_PASS");
    return 0;
}
int main()
{
    int fail = 0;
    if (TestOutputWrite()) return 204;

    // The base stream starts empty.
    fable_u8 raw[sizeof(CDataInputStream)];
    memset(raw, 0xA5, sizeof(raw));
    CDataInputStream* base = new (raw) CDataInputStream;
    if (base->StreamPos != 0 || base->StreamSize != 0 || base->CurrentSourcePtr != 0 ||
        base->SourceChunkStreamPos != 0 || base->SourceChunkBytesRemaining != 0)
        fail |= 1;

    FakeFile file;

    // No buffer: nothing allocated, the file is only remembered.
    fable_u8 rawFile[sizeof(CFileDataInputStream)];
    memset(rawFile, 0xA5, sizeof(rawFile));
    g_sequence = 0; g_newAt = 0; g_setPositionAt = 0;
    CFileDataInputStream* unbuffered = new (rawFile) CFileDataInputStream(&file, 0);
    if (unbuffered->File != &file || unbuffered->StreamPos != 0 || unbuffered->StreamSize != 0 ||
        unbuffered->CurrentSourcePtr != 0 || g_newAt != 0 || g_setPositionAt != 0 ||
        unbuffered->Buffer != (fable_u8*)0xA5A5A5A5)
        fail |= 2;

    // A buffer: window allocated, length cached, positioned at the file's position.
    memset(rawFile, 0xA5, sizeof(rawFile));
    g_sequence = 0; g_newAt = 0; g_setPositionAt = 0;
    g_stream = reinterpret_cast<CFileDataInputStream*>(rawFile);
    CFileDataInputStream* buffered = new (rawFile) CFileDataInputStream(&file, 0x4000);
    if (buffered->File != &file || buffered->Buffer != g_buffer || buffered->BufferSize != 0x4000 ||
        g_newSize != 0x4000 || g_newAt != 1 || g_setPositionAt != 4 || g_setPositionValue != 77 ||
        !g_windowReadyAtSetPosition)
        fail |= 4;

    // Memory streams: the inline default constructor leaves no source; OpenMemorySource
    // and the (data, size) and string constructors open one.
    static const char text[] = "line one\nline two";
    fable_u8 rawMemory[sizeof(CMemoryDataInputStream)];
    memset(rawMemory, 0xA5, sizeof(rawMemory));
    CMemoryDataInputStream* empty = new (rawMemory) CMemoryDataInputStream;
    void* memoryVtable = *reinterpret_cast<void**>(empty);
    if (empty->StreamSource != 0 || empty->StreamPos != 0 || empty->StreamSize != 0)
        fail |= 8;
    empty->OpenMemorySource(text, 9);
    if (!OpenOver(empty, text, 9))
        fail |= 16;
    for (unsigned long size = 0; size < 20; size += 6)
    {
        memset(rawMemory, 0xA5, sizeof(rawMemory));
        CMemoryDataInputStream* stream = new (rawMemory) CMemoryDataInputStream(text, size);
        if (!OpenOver(stream, text, size) || *reinterpret_cast<void**>(stream) != memoryVtable)
            fail |= 32;
        if (size && stream->ReadValue<char>() != 'l')
            fail |= 32;
    }
    // A string's characters (the shared empty string for a null one).
    unsigned char stringData[sizeof(CCharStringData)];
    memset(stringData, 0, sizeof(stringData));
    reinterpret_cast<CCharStringData*>(stringData)->SetText_(const_cast<char*>(text));
    reinterpret_cast<CCharStringData*>(stringData)->SetLength_(8);
    union { unsigned char bytes[sizeof(CCharString)]; double align; } source;
    memset(&source, 0, sizeof(source));
    CCharString& value = *reinterpret_cast<CCharString*>(&source);
    value.storage_ = reinterpret_cast<CCharStringData*>(stringData);
    memset(rawMemory, 0xA5, sizeof(rawMemory));
    CMemoryDataInputStream* fromString = new (rawMemory) CMemoryDataInputStream(value);
    if (!OpenOver(fromString, text, 8) || *reinterpret_cast<void**>(fromString) != memoryVtable)
        fail |= 64;
    value.storage_ = 0;
    memset(rawMemory, 0xA5, sizeof(rawMemory));
    CMemoryDataInputStream* fromEmpty = new (rawMemory) CMemoryDataInputStream(value);
    if (!OpenOver(fromEmpty, g_FableEmptyNarrowString_0129AAF4, 0))
        fail |= 128;

    // The input borrows all written bytes, independently of the output's cursor.
    // Empty output borrows the array object's address, even if capacity is allocated.
    int copyCases = 0;
    for (unsigned long length = 0; length <= 32; ++length)
    for (unsigned long position = 0; position <= 40; position += 10)
    {
        union { unsigned char bytes[sizeof(CMemoryDataOutputStream)]; double align; } rawOutput;
        memset(&rawOutput, 0, sizeof(rawOutput));
        CMemoryDataOutputStream& output = *reinterpret_cast<CMemoryDataOutputStream*>(&rawOutput);
        char buffer[64] = { 0 };
        output.Data._M_start = buffer;
        output.Data._M_finish = buffer + length;
        output.Data._M_end_of_storage._M_data = buffer + 64;
        output.CurrentPosition = position;
        if (output.GetLength() != length) return 205;
        const void* expected = length ? static_cast<const void*>(buffer) : static_cast<const void*>(&output.Data);
        memset(rawMemory, 0xA5, sizeof(rawMemory));
        CMemoryDataInputStream* copy = new (rawMemory) CMemoryDataInputStream(output);
        if (!OpenOver(copy, expected, length) || *reinterpret_cast<void**>(copy) != memoryVtable ||
            output.CurrentPosition != position || output.Data._M_start != buffer || output.Data._M_finish != buffer + length)
            return 201;
        void* slots[5] = { 0,0,0,0,reinterpret_cast<void*>(&CaptureWrite) };
        union { unsigned char bytes[sizeof(CDataOutputStream)]; double align; } rawWriter;
        memset(&rawWriter, 0, sizeof(rawWriter));
        *reinterpret_cast<void***>(&rawWriter) = slots;
        CDataOutputStream& writer = *reinterpret_cast<CDataOutputStream*>(&rawWriter);
        g_writeCalls = 0; writer.Write(output);
        if (g_writeCalls != 1 || g_writer != &writer || g_written != expected || g_writtenSize != (long)length)
            return 202;
        ++copyCases;
    }
    // No storage and no bytes has the same array-object sentinel.
    union { unsigned char bytes[sizeof(CMemoryDataOutputStream)]; double align; } nullOutput;
    memset(&nullOutput, 0, sizeof(nullOutput));
    CMemoryDataOutputStream& nullSource = *reinterpret_cast<CMemoryDataOutputStream*>(&nullOutput);
    CMemoryDataInputStream* nullCopy = new (rawMemory) CMemoryDataInputStream(nullSource);
    if (!OpenOver(nullCopy, &nullSource.Data, 0)) return 203;
    printf("MEMORY_STREAM_COPY_CASES=%d\n", copyCases + 1);

    if (fail == 0)
        printf("FABLETLC_DATAINPUTSTREAM_CTORS_00993B20_PASS\n");
    else
        printf("FABLETLC_DATAINPUTSTREAM_CTORS_00993B20 FAIL %d\n", fail);
    return fail;
}
