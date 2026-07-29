#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/99/CDataInputStream_ReadWithSrcChunkOverflow_00993ca0.cpp"

struct TestDataInputStream_00993ca0
{
    void** vtable;
    long position;
    long totalSize;
    unsigned char* source;
    long sourcePosition;
    long sourceBytes;
};

static unsigned char g_Chunk0[8];
static unsigned char g_Chunk1[8];
static int g_NeedsBuffered;
static int g_NeedsCalls;
static long g_NeedsSize;
static int g_NextChunkCalls;
static int g_UnbufferedCalls;
static unsigned char* g_UnbufferedDestination;
static long g_UnbufferedSize;

extern "C" unsigned char __fastcall
TestNeedsBufferedRead_00993ca0(void*, void*, long size)
{
    ++g_NeedsCalls;
    g_NeedsSize = size;
    return (unsigned char)g_NeedsBuffered;
}

extern "C" void __fastcall
TestInternalReadNextSourceChunk_00993ca0(
    void* streamValue,
    void*,
    unsigned char** source,
    long* sourceBytes)
{
    TestDataInputStream_00993ca0* stream =
        (TestDataInputStream_00993ca0*)streamValue;
    if (g_NextChunkCalls == 0)
    {
        *source = g_Chunk0;
        *sourceBytes = 3;
    }
    else
    {
        *source = g_Chunk1;
        *sourceBytes = 5;
    }
    ++g_NextChunkCalls;

    // Retail records the absolute source position before every refill.
    if (stream->sourcePosition != stream->position)
    {
        g_NextChunkCalls = 100;
    }
}

extern "C" void __fastcall
TestInternalReadUnbuffered_00993ca0(
    void*,
    void*,
    unsigned char* destination,
    long size)
{
    ++g_UnbufferedCalls;
    g_UnbufferedDestination = destination;
    g_UnbufferedSize = size;
    for (long index = 0; index < size; ++index)
    {
        destination[index] = (unsigned char)(0xD0 + index);
    }
}

static void ResetProbes()
{
    g_NeedsBuffered = 0;
    g_NeedsCalls = 0;
    g_NeedsSize = -1;
    g_NextChunkCalls = 0;
    g_UnbufferedCalls = 0;
    g_UnbufferedDestination = 0;
    g_UnbufferedSize = -1;
}

int main()
{
    void* vtable[11];
    std::memset(vtable, 0, sizeof(vtable));
    vtable[8] = (void*)&TestInternalReadNextSourceChunk_00993ca0;
    vtable[9] = (void*)&TestNeedsBufferedRead_00993ca0;
    vtable[10] = (void*)&TestInternalReadUnbuffered_00993ca0;

    typedef void (__fastcall *ReadFunction)(
        void*,
        void*,
        unsigned char*,
        long);
    ReadFunction volatile read =
        &CDataInputStream_ReadWithSrcChunkOverflow_00993ca0;

    int failures = 0;
    unsigned char initial[] = { 0x11, 0x22, 0x33 };
    unsigned char destination[16];
    std::memset(destination, 0, sizeof(destination));

    TestDataInputStream_00993ca0 stream;
    stream.vtable = vtable;
    stream.position = 10;
    stream.totalSize = 100;
    stream.source = initial;
    stream.sourcePosition = 7;
    stream.sourceBytes = 3;

    ResetProbes();
    read(&stream, 0, destination, 8);
    if (g_NeedsCalls != 1 ||
        g_NeedsSize != 5 ||
        g_UnbufferedCalls != 1 ||
        g_UnbufferedDestination != destination + 3 ||
        g_UnbufferedSize != 5 ||
        g_NextChunkCalls != 0 ||
        destination[0] != 0x11 ||
        destination[1] != 0x22 ||
        destination[2] != 0x33 ||
        destination[3] != 0xD0 ||
        destination[7] != 0xD4 ||
        stream.position != 18 ||
        stream.source != 0 ||
        stream.sourcePosition != 0 ||
        stream.sourceBytes != 0)
    {
        ++failures;
    }

    for (int index = 0; index < 8; ++index)
    {
        g_Chunk0[index] = (unsigned char)(0x40 + index);
        g_Chunk1[index] = (unsigned char)(0x70 + index);
    }
    std::memset(destination, 0, sizeof(destination));
    stream.position = 0;
    stream.totalSize = 100;
    stream.source = 0;
    stream.sourcePosition = -1;
    stream.sourceBytes = 0;

    ResetProbes();
    g_NeedsBuffered = 1;
    read(&stream, 0, destination, 7);
    if (g_NeedsCalls != 1 ||
        g_NeedsSize != 7 ||
        g_NextChunkCalls != 2 ||
        g_UnbufferedCalls != 0 ||
        destination[0] != 0x40 ||
        destination[1] != 0x41 ||
        destination[2] != 0x42 ||
        destination[3] != 0x70 ||
        destination[6] != 0x73 ||
        stream.position != 7 ||
        stream.source != g_Chunk1 + 4 ||
        stream.sourcePosition != 3 ||
        stream.sourceBytes != 1)
    {
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "DATA_INPUT_OVERFLOW_00993ca0_TEST FAIL count=%d needs=%d next=%d unbuffered=%d position=%ld\n",
            failures,
            g_NeedsCalls,
            g_NextChunkCalls,
            g_UnbufferedCalls,
            stream.position);
        return 1;
    }

    std::printf("DATA_INPUT_OVERFLOW_00993ca0_TEST PASS\n");
    return 0;
}
