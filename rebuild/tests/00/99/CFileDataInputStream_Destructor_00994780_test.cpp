// Behaviour fixture for CFileDataInputStream::~CFileDataInputStream (0x00994780):
// Close runs on the file stream (with its own vtable in place), then the base
// destruction reaches CBaseClassNonCopyable.
#include <stdio.h>
#include <string.h>
#include <new>

#include "fable_data_input_stream.h"
#include "test_support/base_class_stubs.h"
#define FABLE_STUBS_NO_FILE_DATA_INPUT_STREAM_CLOSE
#include "test_support/data_input_stream_stubs.h"

namespace
{
    int g_sequence, g_closeAt, g_baseDestroyAt;
    const void* g_closeThis;
    const void* g_closeVtable;
    const void* g_baseThis;
}

CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable()
{
    g_baseDestroyAt = ++g_sequence;
    g_baseThis = this;
}
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
void CFileDataInputStream::Close()
{
    g_closeAt = ++g_sequence;
    g_closeThis = this;
    g_closeVtable = *reinterpret_cast<void* const*>(this);
}

int main()
{
    fable_u8 raw[sizeof(CFileDataInputStream)];
    CFileDataInputStream* s = new (raw) CFileDataInputStream(0, 0);
    const void* fileVtable = *reinterpret_cast<void* const*>(s);
    g_sequence = 0;
    s->~CFileDataInputStream();
    const bool passed = g_closeAt == 1 && g_closeThis == raw && g_closeVtable == fileVtable &&
                        g_baseDestroyAt == 2 && g_baseThis == raw &&
                        *reinterpret_cast<void* const*>(raw) != fileVtable;
    if (!passed)
    {
        printf("FABLETLC_FILEDATAINPUTSTREAM_DTOR_00994780 FAIL\n");
        return 1;
    }
    printf("FABLETLC_FILEDATAINPUTSTREAM_DTOR_00994780_PASS\n");
    return 0;
}
