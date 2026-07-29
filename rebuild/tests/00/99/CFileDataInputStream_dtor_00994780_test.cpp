#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/99/CFileDataInputStream_dtor_00994780.cpp"

struct TestFileDataInputStream_00994780
{
    void* vtable;
    unsigned char state[32];
};

extern "C" void* CFileDataInputStream_DerivedVTable_00994780[1] = {
    (void*)0x13572468
};
extern "C" void* CFileDataInputStream_BaseVTable_00994780[1] = {
    (void*)0x24681357
};

static int g_Sequence;
static int g_CloseSequence;
static int g_BaseSequence;
static int g_CloseVTableReady;
static int g_BaseVTableReady;
static void* g_StreamSeen;

extern "C" void __fastcall
CFileDataInputStream_Close_00994780(void* stream, void*)
{
    g_CloseSequence = ++g_Sequence;
    g_StreamSeen = stream;
    g_CloseVTableReady =
        *(void**)stream == CFileDataInputStream_DerivedVTable_00994780;
}

extern "C" void __fastcall
CFileDataInputStream_DestroyBase_00994780(void* stream, void*)
{
    g_BaseSequence = ++g_Sequence;
    g_BaseVTableReady =
        *(void**)stream == CFileDataInputStream_BaseVTable_00994780;
}

int main()
{
    TestFileDataInputStream_00994780 stream;
    std::memset(&stream, 0xA5, sizeof(stream));
    unsigned char originalState[sizeof(stream.state)];
    std::memcpy(originalState, stream.state, sizeof(originalState));

    g_Sequence = 0;
    g_CloseSequence = 0;
    g_BaseSequence = 0;
    g_CloseVTableReady = 0;
    g_BaseVTableReady = 0;
    g_StreamSeen = 0;

    typedef void (__fastcall *DestroyFunction)(void*, void*);
    DestroyFunction volatile destroy = &CFileDataInputStream_dtor_00994780;
    destroy(&stream, 0);

    int failures = 0;
    if (g_Sequence != 2 ||
        g_CloseSequence != 1 ||
        g_BaseSequence != 2 ||
        g_CloseVTableReady == 0 ||
        g_BaseVTableReady == 0 ||
        g_StreamSeen != &stream ||
        stream.vtable != CFileDataInputStream_BaseVTable_00994780 ||
        std::memcmp(stream.state, originalState, sizeof(originalState)) != 0)
    {
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FILE_DATA_STREAM_DTOR_00994780_TEST FAIL count=%d sequence=%d\n",
            failures,
            g_Sequence);
        return 1;
    }

    std::printf("FILE_DATA_STREAM_DTOR_00994780_TEST PASS\n");
    return 0;
}
