#include "rebuild_abi.h"

#include <stdio.h>
#include <string.h>

struct FableCharString_0099F2E0
{
    void* storage;
};

extern "C" void FABLE_FASTCALL
FableCharStringLoadFromFile_0099F2E0(
    FableCharString_0099F2E0* self,
    void*,
    void* file,
    void* suspendableProcess,
    long length);

namespace
{
    struct TestFile
    {
        void** vtable;
    };

    unsigned char g_storage[17];
    char g_buffer[128];
    void* g_vtable[10];
    unsigned int g_releaseCalls;
    unsigned int g_allocateCalls;
    unsigned int g_initialiseCalls;
    unsigned int g_sizeCalls;
    unsigned int g_positionCalls;
    unsigned int g_readCalls;
    unsigned long g_allocateSize;
    long g_initialiseLength;
    unsigned long g_initialiseGrowth;
    void* g_readBuffer;
    long g_readLength;
    void* g_readProcess;
    long g_fileSize;
    long g_filePosition;
    bool g_failAllocation;

    long FABLE_FASTCALL SizeStub(TestFile*, void*)
    {
        ++g_sizeCalls;
        return g_fileSize;
    }

    long FABLE_FASTCALL PositionStub(TestFile*, void*)
    {
        ++g_positionCalls;
        return g_filePosition;
    }

    void FABLE_FASTCALL ReadStub(
        TestFile*,
        void*,
        void* buffer,
        long length,
        void* process)
    {
        ++g_readCalls;
        g_readBuffer = buffer;
        g_readLength = length;
        g_readProcess = process;
    }

    void Reset()
    {
        memset(g_storage, 0xFF, sizeof(g_storage));
        memset(g_buffer, 0, sizeof(g_buffer));
        memset(g_vtable, 0, sizeof(g_vtable));
        g_vtable[3] = reinterpret_cast<void*>(&ReadStub);
        g_vtable[7] = reinterpret_cast<void*>(&PositionStub);
        g_vtable[9] = reinterpret_cast<void*>(&SizeStub);
        g_releaseCalls = 0;
        g_allocateCalls = 0;
        g_initialiseCalls = 0;
        g_sizeCalls = 0;
        g_positionCalls = 0;
        g_readCalls = 0;
        g_allocateSize = 0;
        g_initialiseLength = 0;
        g_initialiseGrowth = 0;
        g_readBuffer = 0;
        g_readLength = 0;
        g_readProcess = 0;
        g_fileSize = 91;
        g_filePosition = 27;
        g_failAllocation = false;
    }
}

extern "C" void FABLE_FASTCALL
FableCharStringRelease_0099E9B0(
    FableCharString_0099F2E0* self,
    void*)
{
    ++g_releaseCalls;
    self->storage = 0;
}

extern "C" void* FABLE_CDECL
FableCharStringAllocate_00BFEA1A(unsigned long size)
{
    ++g_allocateCalls;
    g_allocateSize = size;
    return g_failAllocation ? 0 : g_storage;
}

extern "C" void FABLE_FASTCALL
FableCharStringStorageInitialise_009A0390(
    void* storage,
    void*,
    long length,
    unsigned long growth)
{
    ++g_initialiseCalls;
    g_initialiseLength = length;
    g_initialiseGrowth = growth;
    *static_cast<void**>(storage) = g_buffer;
}

int main()
{
    TestFile file = {g_vtable};
    int processToken;
    FableCharString_0099F2E0 text;

    Reset();
    text.storage = reinterpret_cast<void*>(0x11111111);
    FableCharStringLoadFromFile_0099F2E0(
        &text,
        0,
        &file,
        &processToken,
        -1);
    if (
        g_releaseCalls != 1 ||
        g_allocateCalls != 1 ||
        g_allocateSize != 17 ||
        g_sizeCalls != 1 ||
        g_positionCalls != 1 ||
        g_initialiseCalls != 1 ||
        g_initialiseLength != 64 ||
        g_initialiseGrowth != 0x20 ||
        g_readCalls != 1 ||
        g_readBuffer != g_buffer ||
        g_readLength != 64 ||
        g_readProcess != &processToken ||
        text.storage != g_storage)
    {
        return 1;
    }
    if (
        (*reinterpret_cast<unsigned long*>(&g_storage[8]) &
            0x7FFFFFFFul) != 0 ||
        (g_storage[12] & 1) == 0 ||
        *reinterpret_cast<unsigned long*>(&g_storage[13]) != 1)
    {
        return 2;
    }

    Reset();
    text.storage = 0;
    FableCharStringLoadFromFile_0099F2E0(
        &text,
        0,
        &file,
        &processToken,
        12);
    if (
        g_releaseCalls != 0 ||
        g_sizeCalls != 0 ||
        g_positionCalls != 0 ||
        g_initialiseLength != 12 ||
        g_readCalls != 1 ||
        g_readLength != 12)
    {
        return 3;
    }

    Reset();
    g_failAllocation = true;
    text.storage = 0;
    FableCharStringLoadFromFile_0099F2E0(
        &text,
        0,
        &file,
        &processToken,
        12);
    if (
        text.storage != 0 ||
        g_initialiseCalls != 0 ||
        g_readCalls != 0)
    {
        return 4;
    }

    printf("FABLETLC_CHAR_STRING_LOAD_FILE_0099F2E0 PASS\n");
    return 0;
}
