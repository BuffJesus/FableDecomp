#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/99/CDataInputStream_ReadZlibCompressed_00996230.cpp"

struct TestDataInputStream_00996230
{
    void* vtable;
    long position;
    long totalSize;
    unsigned char* source;
    long sourcePosition;
    long sourceBytes;
};

struct TestByteVector_00996230
{
    unsigned char* begin;
    unsigned char* end;
    unsigned char* capacityEnd;
};

static unsigned char g_CompressedStorage[16];
static int g_Sequence;
static int g_VectorSequence;
static int g_BaseSequence;
static int g_ConstructSequence;
static int g_InitSequence;
static int g_DecompressSequence;
static int g_DestroySequence;
static int g_FreeSequence;
static int g_OverflowCalls;
static unsigned long g_VectorSize;
static void* g_DestinationSeen;
static unsigned long g_OutputSizeSeen;
static unsigned long g_DecompressReturn;
static int g_CallbackCalls;
static int g_VTableReady;

extern "C" void* CDataInputStream_CompressorVTable_00996230[1] = {
    (void*)0x13572468
};
extern "C" void* CDataInputStream_ZlibSizeMismatchCallback_00996230 = 0;

extern "C" void __fastcall
CDataInputStream_ReadOverflow_00996230(
    void*,
    void*,
    unsigned char*,
    long)
{
    ++g_OverflowCalls;
}

extern "C" void __fastcall
CDataInputStream_InitCompressedVector_00996230(
    void* vectorValue,
    void*,
    unsigned long size)
{
    g_VectorSequence = ++g_Sequence;
    g_VectorSize = size;
    TestByteVector_00996230* vector =
        (TestByteVector_00996230*)vectorValue;
    vector->begin = g_CompressedStorage;
    vector->end = g_CompressedStorage + size;
    vector->capacityEnd = vector->end;
}

extern "C" void* __cdecl
CDataInputStream_ConstructCompressorBase_00996230()
{
    g_BaseSequence = ++g_Sequence;
    return (void*)0x24681357;
}

extern "C" void __fastcall
CDataInputStream_ConstructCompressor_00996230(
    void*,
    void*,
    void* base)
{
    g_ConstructSequence = ++g_Sequence;
    if (base != (void*)0x24681357)
    {
        g_ConstructSequence = 100;
    }
}

extern "C" void __fastcall
CDataInputStream_InitCompressor_00996230(
    void* compressor,
    void*)
{
    g_InitSequence = ++g_Sequence;
    g_VTableReady =
        *(void**)compressor == CDataInputStream_CompressorVTable_00996230;
}

extern "C" unsigned long __fastcall
CDataInputStream_InternalDecompress_00996230(
    void* compressor,
    void*,
    const void* compressed,
    unsigned long compressedSize,
    void* destination,
    unsigned long outputSize)
{
    g_DecompressSequence = ++g_Sequence;
    g_DestinationSeen = destination;
    g_OutputSizeSeen = outputSize;
    if (*(void**)compressor != CDataInputStream_CompressorVTable_00996230 ||
        compressed != g_CompressedStorage ||
        compressedSize != 3 ||
        g_CompressedStorage[0] != 0xA1 ||
        g_CompressedStorage[1] != 0xB2 ||
        g_CompressedStorage[2] != 0xC3)
    {
        g_DecompressSequence = 100;
    }
    unsigned char* output = (unsigned char*)destination;
    for (unsigned long index = 0; index < outputSize; ++index)
    {
        output[index] = (unsigned char)(0x50 + index);
    }
    return g_DecompressReturn;
}

extern "C" void __fastcall
CDataInputStream_DestroyCompressor_00996230(void*, void*)
{
    g_DestroySequence = ++g_Sequence;
}

extern "C" void __cdecl
CDataInputStream_FreeCompressedBuffer_00996230(void* buffer)
{
    g_FreeSequence = ++g_Sequence;
    if (buffer != g_CompressedStorage)
    {
        g_FreeSequence = 100;
    }
}

extern "C" void __cdecl TestSizeMismatchCallback_00996230()
{
    ++g_CallbackCalls;
}

static void ResetProbes(unsigned long decompressedSize)
{
    g_Sequence = 0;
    g_VectorSequence = 0;
    g_BaseSequence = 0;
    g_ConstructSequence = 0;
    g_InitSequence = 0;
    g_DecompressSequence = 0;
    g_DestroySequence = 0;
    g_FreeSequence = 0;
    g_OverflowCalls = 0;
    g_VectorSize = 0;
    g_DestinationSeen = 0;
    g_OutputSizeSeen = 0;
    g_DecompressReturn = decompressedSize;
    g_CallbackCalls = 0;
    g_VTableReady = 0;
}

int main()
{
    typedef void (__fastcall *ReadFunction)(
        void*,
        void*,
        void*,
        long);
    ReadFunction volatile read =
        &CDataInputStream_ReadZlibCompressed_00996230;

    unsigned char source[7];
    unsigned long compressedSize = 3;
    std::memcpy(source, &compressedSize, 4);
    source[4] = 0xA1;
    source[5] = 0xB2;
    source[6] = 0xC3;
    unsigned char destination[12];
    std::memset(destination, 0, sizeof(destination));

    TestDataInputStream_00996230 stream;
    stream.vtable = 0;
    stream.position = 10;
    stream.totalSize = 100;
    stream.source = source;
    stream.sourcePosition = 10;
    stream.sourceBytes = 7;

    int failures = 0;
    ResetProbes(7);
    CDataInputStream_ZlibSizeMismatchCallback_00996230 = 0;
    read(&stream, 0, destination, 7);
    if (g_Sequence != 7 ||
        g_VectorSequence != 1 ||
        g_BaseSequence != 2 ||
        g_ConstructSequence != 3 ||
        g_InitSequence != 4 ||
        g_DecompressSequence != 5 ||
        g_DestroySequence != 6 ||
        g_FreeSequence != 7 ||
        g_OverflowCalls != 0 ||
        g_VectorSize != 3 ||
        g_DestinationSeen != destination ||
        g_OutputSizeSeen != 7 ||
        g_CallbackCalls != 0 ||
        g_VTableReady == 0 ||
        destination[0] != 0x50 ||
        destination[6] != 0x56 ||
        stream.position != 17 ||
        stream.source != source + 7 ||
        stream.sourceBytes != 0)
    {
        ++failures;
    }

    stream.position = 0;
    stream.source = source;
    stream.sourcePosition = 0;
    stream.sourceBytes = 7;
    ResetProbes(6);
    CDataInputStream_ZlibSizeMismatchCallback_00996230 =
        (void*)&TestSizeMismatchCallback_00996230;
    read(&stream, 0, destination, 7);
    if (g_CallbackCalls != 1 ||
        g_DestroySequence != 6 ||
        g_FreeSequence != 7)
    {
        ++failures;
    }

    ResetProbes(0);
    read(&stream, 0, destination, 0);
    if (g_Sequence != 0 ||
        g_OverflowCalls != 0 ||
        g_CallbackCalls != 0)
    {
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "READ_ZLIB_00996230_TEST FAIL count=%d sequence=%d overflow=%d callback=%d\n",
            failures,
            g_Sequence,
            g_OverflowCalls,
            g_CallbackCalls);
        return 1;
    }

    std::printf("READ_ZLIB_00996230_TEST PASS\n");
    return 0;
}
