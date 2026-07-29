#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/41/CMainGameComponentInit_CMainGameComponentInit_004131a0.cpp"

static unsigned char* volatile g_Base;
static volatile int g_CallCount;
static volatile int g_Offsets[4];
static volatile char g_Kinds[4];

extern "C" void* __fastcall
CMainGameComponentInit_WideCtor_004131a0(void* destination, void*)
{
    const int call = g_CallCount++;
    g_Offsets[call] = (int)((unsigned char*)destination - g_Base);
    g_Kinds[call] = 'W';
    *(void**)destination = (void*)(call + 1);
    return destination;
}

extern "C" void* __fastcall
CMainGameComponentInit_NarrowCtor_004131a0(void* destination, void*)
{
    const int call = g_CallCount++;
    g_Offsets[call] = (int)((unsigned char*)destination - g_Base);
    g_Kinds[call] = 'N';
    *(void**)destination = (void*)(call + 1);
    return destination;
}

int main()
{
    unsigned char record[16];
    std::memset(record, 0, sizeof(record));
    for (int index = 0; index != 4; ++index)
    {
        g_Offsets[index] = -1;
        g_Kinds[index] = 0;
    }
    g_Base = record;
    g_CallCount = 0;

    typedef void* (__fastcall *CtorFunction)(void*, void*);
    CtorFunction volatile constructor =
        &CMainGameComponentInit_CMainGameComponentInit_004131a0;
    void* result = constructor(record, 0);

    const int expectedOffsets[4] = { 0, 4, 8, 12 };
    const char expectedKinds[4] = { 'W', 'W', 'N', 'W' };
    int failures = 0;

    if (result != record || g_CallCount != 4)
        ++failures;

    for (int index = 0; index != 4; ++index)
    {
        if (g_Offsets[index] != expectedOffsets[index] ||
            g_Kinds[index] != expectedKinds[index] ||
            *(void**)(record + index * 4) != (void*)(index + 1))
        {
            ++failures;
        }
    }

    if (failures != 0)
    {
        std::printf(
            "MAIN_GAME_INIT_004131a0_TEST FAIL count=%d calls=%d\n",
            failures,
            g_CallCount);
        return 1;
    }

    std::printf("MAIN_GAME_INIT_004131a0_TEST PASS\n");
    return 0;
}
