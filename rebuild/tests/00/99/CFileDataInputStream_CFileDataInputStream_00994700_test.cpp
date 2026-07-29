#include <cstdio>

#include "../../../src/compiled/00/99/CFileDataInputStream_CFileDataInputStream_00994700.cpp"

struct TestFile_00994700
{
    void** vtable;
};

struct TestFileDataInputStream_00994700
{
    void* vtable;
    unsigned long field04;
    unsigned long field08;
    void* field0c;
    unsigned long field10;
    unsigned long field14;
    void* file;
    void* buffer;
    unsigned long capacity;
};

extern "C" void* CFileDataInputStream_VTable_00994700[1] = {
    (void*)0x24681357
};

static unsigned char g_Buffer[64];
static volatile int g_Sequence;
static volatile int g_BaseSequence;
static volatile int g_AllocateSequence;
static volatile int g_GetPositionSequence;
static volatile int g_GetSizeSequence;
static volatile int g_SetPositionSequence;
static volatile unsigned long g_AllocatedSize;
static volatile unsigned long g_PositionSeen;
static volatile int g_FieldsReadyAtSetPosition;
static TestFileDataInputStream_00994700* volatile g_Stream;
static TestFile_00994700* volatile g_File;

extern "C" void* __fastcall
CFileDataInputStream_ConstructBase_00994700(void* stream, void*)
{
    g_BaseSequence = ++g_Sequence;
    return stream;
}

extern "C" void* __cdecl
CFileDataInputStream_AllocateBuffer_00994700(unsigned long size)
{
    g_AllocateSequence = ++g_Sequence;
    g_AllocatedSize = size;
    return g_Buffer;
}

extern "C" unsigned long __fastcall
TestFileGetPosition_00994700(void* file, void*)
{
    g_GetPositionSequence = ++g_Sequence;
    return file == g_File ? 7 : 0;
}

extern "C" unsigned long __fastcall
TestFileGetSize_00994700(void* file, void*)
{
    g_GetSizeSequence = ++g_Sequence;
    return file == g_File ? 41 : 0;
}

extern "C" void __fastcall
CFileDataInputStream_SetPosition_00994700(
    void* stream,
    void*,
    unsigned long position)
{
    g_SetPositionSequence = ++g_Sequence;
    TestFileDataInputStream_00994700* value =
        (TestFileDataInputStream_00994700*)stream;
    g_PositionSeen = position;
    g_FieldsReadyAtSetPosition =
        value == g_Stream &&
        value->vtable == CFileDataInputStream_VTable_00994700 &&
        value->file == g_File &&
        value->buffer == g_Buffer &&
        value->capacity == 32 &&
        value->field04 == 0 &&
        value->field08 == 41 &&
        value->field0c == g_Buffer &&
        value->field10 == 0 &&
        value->field14 == 0;
}

static void ResetProbe()
{
    g_Sequence = 0;
    g_BaseSequence = 0;
    g_AllocateSequence = 0;
    g_GetPositionSequence = 0;
    g_GetSizeSequence = 0;
    g_SetPositionSequence = 0;
    g_AllocatedSize = 0;
    g_PositionSeen = 0;
    g_FieldsReadyAtSetPosition = 0;
}

int main()
{
    void* fileVtable[10] = { 0 };
    fileVtable[7] = (void*)&TestFileGetPosition_00994700;
    fileVtable[9] = (void*)&TestFileGetSize_00994700;
    TestFile_00994700 file;
    file.vtable = fileVtable;
    g_File = &file;

    typedef void* (__fastcall *ConstructorFunction)(
        void*,
        void*,
        void*,
        unsigned long);
    ConstructorFunction volatile construct =
        &CFileDataInputStream_CFileDataInputStream_00994700;

    TestFileDataInputStream_00994700 stream;
    stream.buffer = (void*)0xaaaaaaaa;
    stream.capacity = 0xbbbbbbbb;
    g_Stream = &stream;
    ResetProbe();
    void* result = construct(&stream, 0, &file, 32);

    int failures = 0;
    if (result != &stream ||
        g_BaseSequence != 1 ||
        g_AllocateSequence != 2 ||
        g_GetSizeSequence != 3 ||
        g_GetPositionSequence != 4 ||
        g_SetPositionSequence != 5 ||
        g_AllocatedSize != 32 ||
        g_PositionSeen != 7 ||
        g_FieldsReadyAtSetPosition != 1)
    {
        ++failures;
    }

    stream.buffer = (void*)0xaaaaaaaa;
    stream.capacity = 0xbbbbbbbb;
    g_Stream = &stream;
    ResetProbe();
    result = construct(&stream, 0, &file, 0);

    if (result != &stream ||
        g_BaseSequence != 1 ||
        g_AllocateSequence != 0 ||
        g_GetPositionSequence != 0 ||
        g_GetSizeSequence != 0 ||
        g_SetPositionSequence != 0 ||
        stream.vtable != CFileDataInputStream_VTable_00994700 ||
        stream.file != &file ||
        stream.field04 != 0 ||
        stream.field08 != 0 ||
        stream.field0c != 0 ||
        stream.field10 != 0 ||
        stream.field14 != 0 ||
        stream.buffer != (void*)0xaaaaaaaa ||
        stream.capacity != 0xbbbbbbbb)
    {
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FILE_DATA_STREAM_00994700_TEST FAIL count=%d sequence=%d\n",
            failures,
            g_Sequence);
        return 1;
    }

    std::printf("FILE_DATA_STREAM_00994700_TEST PASS\n");
    return 0;
}
