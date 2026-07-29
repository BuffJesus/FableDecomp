#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/4a/CWorld_PrepareForLoad_004a0220.cpp"

static int g_GlobalInterface;
static int g_PlayerOwner;
static int g_Player;
static int g_Environment;
static int g_Subsystem18;
static int g_Subsystem60;
static int g_Subsystem68;
static int g_Subsystem6c;
static int g_WorldMap;
static int g_PlayerGui;
static int g_GuiOwner;
static int g_CountedObject;
extern "C" void* CWorld_PrepareForLoad_Global_004a0220 =
    &g_GlobalInterface;

static volatile int g_Sequence;
static volatile int g_GetPlayerCalls;
static volatile int g_RemoveMode;
static volatile int g_Subsystem4Argument;
static volatile int g_EnvironmentArgument;
static volatile int g_ThemeChannels[3];
static volatile int g_ThemeValues[3];
static volatile int g_ThemeCalls;
static volatile int g_ReleaseCalls;
static volatile int g_DeleteCalls;
static volatile int g_EraseCalls;
static void* volatile g_EraseFirst;
static void* volatile g_EraseLast;
static unsigned char* volatile g_World;
static volatile int g_LateFieldsReady;
static volatile int g_LastSequence;

static void Step()
{
    g_LastSequence = ++g_Sequence;
}

extern "C" void* __fastcall CWorld_Prepare_GetPlayer_004a0220(
    void* owner,
    void*)
{
    Step();
    if (owner == &g_PlayerOwner)
        ++g_GetPlayerCalls;
    return &g_Player;
}

extern "C" void __fastcall CWorld_Prepare_RemovePlayerMode_004a0220(
    void* player,
    void*,
    int mode)
{
    Step();
    if (player == &g_Player)
        g_RemoveMode = mode;
}

extern "C" void __fastcall CWorld_Prepare_ResetPlayer_004a0220(
    void* player,
    void*)
{
    Step();
    if (player != &g_Player)
        g_GetPlayerCalls = -100;
}

extern "C" void* __fastcall CWorld_Prepare_GetEnvironment_004a0220(
    void* owner,
    void*)
{
    Step();
    return owner == &g_GlobalInterface ? &g_Environment : 0;
}

extern "C" void __fastcall CWorld_Prepare_ResetEnvironment_004a0220(
    void* environment,
    void*)
{
    Step();
    if (environment != &g_Environment)
        g_GetPlayerCalls = -101;
}

extern "C" void __fastcall CWorld_Prepare_ResetGlobalInterface_004a0220(
    void* owner,
    void*)
{
    Step();
    if (owner != &g_GlobalInterface)
        g_GetPlayerCalls = -102;
}

#define DEFINE_NOARG_STEP(name, expected) \
extern "C" void __fastcall name(void* receiver, void*) \
{ Step(); if (receiver != expected) g_GetPlayerCalls = -103; }

DEFINE_NOARG_STEP(CWorld_Prepare_Subsystem1_004a0220, &g_Subsystem6c)
DEFINE_NOARG_STEP(CWorld_Prepare_Subsystem2_004a0220, &g_Subsystem6c)
DEFINE_NOARG_STEP(CWorld_Prepare_Subsystem3_004a0220, &g_Subsystem6c)
DEFINE_NOARG_STEP(CWorld_Prepare_Subsystem5_004a0220, &g_Subsystem6c)
DEFINE_NOARG_STEP(CWorld_Prepare_WorldMap_004a0220, &g_WorldMap)
DEFINE_NOARG_STEP(CWorld_Prepare_Subsystem6_004a0220, &g_Subsystem68)
DEFINE_NOARG_STEP(CWorld_Prepare_PlayerGui_004a0220, &g_PlayerGui)

extern "C" void __fastcall CWorld_Prepare_Subsystem7_004a0220(
    void* receiver,
    void*)
{
    Step();
    if (receiver != (unsigned char*)&g_Subsystem60 + 4)
        g_GetPlayerCalls = -104;
}

extern "C" void __fastcall CWorld_Prepare_Subsystem4_004a0220(
    void* receiver,
    void*,
    int argument)
{
    Step();
    if (receiver != &g_Subsystem6c)
        g_GetPlayerCalls = -105;
    g_Subsystem4Argument = argument;
}

extern "C" void __fastcall CWorld_Prepare_SetEnvironmentState_004a0220(
    void* receiver,
    void*,
    int argument)
{
    Step();
    if (receiver != &g_Subsystem18)
        g_GetPlayerCalls = -106;
    g_EnvironmentArgument = argument;
}

extern "C" void __fastcall CWorld_Prepare_SetTheme_004a0220(
    void* receiver,
    void*,
    int channel,
    int value)
{
    Step();
    if (receiver != &g_Subsystem18)
        g_GetPlayerCalls = -107;
    const int call = g_ThemeCalls++;
    g_ThemeChannels[call] = channel;
    g_ThemeValues[call] = value;
}

extern "C" void __cdecl CWorld_Prepare_DeleteCountedBlock_004a0220(
    void*)
{
    Step();
    ++g_DeleteCalls;
}

extern "C" void __fastcall CWorld_Prepare_EraseRange_004a0220(
    void* range,
    void*,
    void* first,
    void* last)
{
    Step();
    ++g_EraseCalls;
    g_EraseFirst = first;
    g_EraseLast = last;
    unsigned char* world = g_World;
    g_LateFieldsReady =
        range == world + 0x168 &&
        *(unsigned long*)(world + 0x8c) == 0 &&
        world[0xd8] == 0 && world[0xdb] == 1 &&
        world[0xdc] == 0 && world[0xdf] == 1 &&
        world[0xe2] == 0 &&
        *(unsigned long*)(world + 0xe4) == 0 &&
        *(unsigned long*)(world + 0x124) == 0 &&
        world[0x140] == 0 && world[0x164] == 0 &&
        *(unsigned long*)(world + 0x178) == 0x77777777;
}

extern "C" void __fastcall CountedRelease_004a0220(void* object, void*)
{
    Step();
    if (object == &g_CountedObject)
        ++g_ReleaseCalls;
}

extern "C" void* __fastcall WorldGetMap_004a0220(void*, void*)
{
    Step();
    return &g_WorldMap;
}

static int RunCase(long initialCount, int expectedRelease, int expectedDelete)
{
    unsigned char world[0x17c];
    unsigned char owner8[0x2c];
    unsigned long vtable[13];
    unsigned long counted[3];
    int rangeValues[2];
    std::memset(world, 0x55, sizeof(world));
    std::memset(owner8, 0, sizeof(owner8));
    std::memset(vtable, 0, sizeof(vtable));

    vtable[12] = (unsigned long)&WorldGetMap_004a0220;
    counted[0] = initialCount;
    counted[1] = (unsigned long)&CountedRelease_004a0220;
    counted[2] = (unsigned long)&g_CountedObject;
    rangeValues[0] = 10;
    rangeValues[1] = 20;

    *(unsigned long**)(world + 0) = vtable;
    *(void**)(world + 0x0c) = &g_PlayerOwner;
    *(void**)(world + 0x18) = &g_Subsystem18;
    *(void**)(world + 0x24) = (void*)0x24242424;
    *(unsigned long**)(world + 0x28) = counted;
    *(void**)(world + 0x60) = &g_Subsystem60;
    *(void**)(world + 0x68) = &g_Subsystem68;
    *(void**)(world + 0x6c) = &g_Subsystem6c;
    *(unsigned char**)(world + 0x08) = owner8;
    *(void**)(owner8 + 0x28) = &g_PlayerGui;
    *(void**)(world + 0x168) = &rangeValues[0];
    *(void**)(world + 0x16c) = &rangeValues[2];
    *(unsigned long*)(world + 0x178) = 0x77777777;

    g_Sequence = 0;
    g_GetPlayerCalls = 0;
    g_RemoveMode = -1;
    g_Subsystem4Argument = -1;
    g_EnvironmentArgument = -1;
    g_ThemeCalls = 0;
    g_ReleaseCalls = 0;
    g_DeleteCalls = 0;
    g_EraseCalls = 0;
    g_EraseFirst = 0;
    g_EraseLast = 0;
    g_World = world;
    g_LateFieldsReady = 0;

    typedef void (__fastcall *PrepareFunction)(void*, void*);
    PrepareFunction volatile prepare = &CWorld_PrepareForLoad_004a0220;
    prepare(world, 0);

    int failures = 0;
    if (g_GetPlayerCalls != 2 ||
        g_RemoveMode != 0x2b ||
        g_Subsystem4Argument != 0 ||
        g_EnvironmentArgument != 1 ||
        g_ThemeCalls != 3 ||
        g_ThemeChannels[0] != 1 || g_ThemeValues[0] != 0 ||
        g_ThemeChannels[1] != 2 || g_ThemeValues[1] != 0 ||
        g_ThemeChannels[2] != 3 || g_ThemeValues[2] != 0 ||
        g_ReleaseCalls != expectedRelease ||
        g_DeleteCalls != expectedDelete ||
        g_EraseCalls != 1 ||
        g_EraseFirst != &rangeValues[0] ||
        g_EraseLast != &rangeValues[2] ||
        g_LateFieldsReady != 1)
    {
        ++failures;
    }

    if (*(void**)(world + 0x24) != 0 ||
        *(void**)(world + 0x28) != 0 ||
        *(unsigned long*)(world + 0x178) != 0)
    {
        ++failures;
    }

    return failures;
}

int main()
{
    const int failures = RunCase(1, 1, 1) + RunCase(2, 0, 0);
    if (failures != 0)
    {
        std::printf(
            "PREPARE_FOR_LOAD_004a0220_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("PREPARE_FOR_LOAD_004a0220_TEST PASS\n");
    return 0;
}
