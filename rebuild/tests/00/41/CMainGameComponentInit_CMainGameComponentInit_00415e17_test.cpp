#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/41/CMainGameComponentInit_CMainGameComponentInit_00415e17.cpp"

static unsigned char* volatile g_DestinationBase;
static const unsigned char* volatile g_SourceBase;
static volatile int g_CallCount;
static volatile int g_DestinationOffsets[4];
static volatile int g_SourceOffsets[4];
static volatile char g_Kinds[4];

static void* ProbeCopy(
    void* destination,
    const void* source,
    char kind)
{
    const int call = g_CallCount++;
    g_DestinationOffsets[call] =
        (int)((unsigned char*)destination - g_DestinationBase);
    g_SourceOffsets[call] =
        (int)((const unsigned char*)source - g_SourceBase);
    g_Kinds[call] = kind;
    *(void**)destination = *(void* const*)source;
    return destination;
}

extern "C" void* __fastcall
CMainGameComponentInit_WideCopyCtor_00415e17(
    void* destination,
    void*,
    const void* source)
{
    return ProbeCopy(destination, source, 'W');
}

extern "C" void* __fastcall
CMainGameComponentInit_NarrowCopyCtor_00415e17(
    void* destination,
    void*,
    const void* source)
{
    return ProbeCopy(destination, source, 'N');
}

int main()
{
    void* source[4] = {
        (void*)0x11111111,
        (void*)0x22222222,
        (void*)0x33333333,
        (void*)0x44444444
    };
    void* destination[4];
    std::memset(destination, 0, sizeof(destination));

    g_DestinationBase = (unsigned char*)destination;
    g_SourceBase = (const unsigned char*)source;
    g_CallCount = 0;
    for (int index = 0; index != 4; ++index)
    {
        g_DestinationOffsets[index] = -1;
        g_SourceOffsets[index] = -1;
        g_Kinds[index] = 0;
    }

    typedef void* (__fastcall *CopyCtorFunction)(
        void*,
        void*,
        const void*);
    CopyCtorFunction volatile constructor =
        &CMainGameComponentInit_CMainGameComponentInit_00415e17;
    void* result = constructor(destination, 0, source);

    const int expectedOffsets[4] = { 0, 4, 8, 12 };
    const char expectedKinds[4] = { 'W', 'W', 'N', 'W' };
    int failures = 0;

    if (result != destination || g_CallCount != 4)
        ++failures;

    for (int index = 0; index != 4; ++index)
    {
        if (g_DestinationOffsets[index] != expectedOffsets[index] ||
            g_SourceOffsets[index] != expectedOffsets[index] ||
            g_Kinds[index] != expectedKinds[index] ||
            destination[index] != source[index])
        {
            ++failures;
        }
    }

    if (failures != 0)
    {
        std::printf(
            "MAIN_GAME_INIT_COPY_00415e17_TEST FAIL count=%d calls=%d\n",
            failures,
            g_CallCount);
        return 1;
    }

    std::printf("MAIN_GAME_INIT_COPY_00415e17_TEST PASS\n");
    return 0;
}
