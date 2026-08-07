#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/59/CFrontEndManager_RefreshAvailableProfiles_00596cce.cpp"

extern "C" const char FableRefreshProfilesDefinition_01252428[] =
    "UI_FRONTEND_LIST_FOR_PROFILES";
extern "C" const char FableRefreshProfilesNewButton_0125240c[] =
    "UI_FRONTEND_BUTTON_FOR_NEW_PROFILE";
extern "C" const char FableRefreshProfilesEmpty_0122d70e[] = "";
extern "C" const char FableRefreshProfilesButton_012523e4[] =
    "UI_FRONTEND_BUTTON";
extern "C" const float FableRefreshProfilesSpacing_01248018 = 30.0f;

struct TestComponent_00596cce
{
    void** vtable;
    unsigned int kind;
};

struct TestAction_00596cce
{
    unsigned int id;
    void* parameter;
};

static void* g_vtable[72];
static TestComponent_00596cce g_root;
static TestComponent_00596cce g_list;
static TestComponent_00596cce g_buttons[5];
static TestComponent_00596cce* g_rootPointer;
static TestComponent_00596cce* g_oldList;
static CFrontEndManager_00596cce g_manager;
static CWideString_00596cce g_profiles[4];
static unsigned char g_valid[4];
static long g_profileCount;
static int g_profileEnumerationCalls;
static int g_buttonCalls;
static int g_actionAllocations;
static int g_parameterAllocations;
static int g_attachCalls;
static int g_appendCalls;
static int g_finishCalls;
static int g_releaseActionCalls;
static int g_releaseButtonCalls;
static int g_releaseListCalls;
static int g_tidyCalls;
static void* g_currentProfile;
static void* g_parameterName;
static TestAction_00596cce g_actions[5];
static unsigned char g_parameters[5][12];

static void Reset(int profileCount)
{
    std::memset(&g_manager, 0, sizeof(g_manager));
    g_profileCount = profileCount;
    g_profileEnumerationCalls = 0;
    g_buttonCalls = 0;
    g_actionAllocations = 0;
    g_parameterAllocations = 0;
    g_attachCalls = 0;
    g_appendCalls = 0;
    g_finishCalls = 0;
    g_releaseActionCalls = 0;
    g_releaseButtonCalls = 0;
    g_releaseListCalls = 0;
    g_tidyCalls = 0;
    g_currentProfile = 0;
    g_parameterName = (void*)0x4a4a0000;
    g_oldList = 0;
    for (int i = 0; i != 4; ++i)
    {
        g_profiles[i].data = (void*)(0x1000 + i);
        g_valid[i] = (i & 1) != 0;
    }
    for (int i = 0; i != 5; ++i)
    {
        g_buttons[i].vtable = g_vtable;
        g_buttons[i].kind = 10 + i;
        g_actions[i].id = 0;
        g_actions[i].parameter = 0;
    }
    g_root.vtable = g_vtable;
    g_root.kind = 1;
    g_list.vtable = g_vtable;
    g_list.kind = 2;
    g_rootPointer = &g_root;
}

extern "C" void* __cdecl FableRefreshProfilesCall_0041e5f2()
{
    return &g_manager;
}

extern "C" void __fastcall FableRefreshProfilesCall_005955ab(
    void*,
    void*,
    CProfileVector_00596cce* output)
{
    ++g_profileEnumerationCalls;
    output->begin = g_profiles;
    output->end = g_profiles + g_profileCount;
    output->capacity = g_profiles + 4;
}

extern "C" void** __fastcall FableRefreshProfilesCall_0059b5d7(
    void*,
    void*,
    const int*)
{
    return (void**)&g_rootPointer;
}

extern "C" void __fastcall FableRefreshProfilesCall_0099ebf0(
    void* destination,
    void*,
    const char* text,
    int)
{
    *(const char**)destination = text;
}

extern "C" void __fastcall FableRefreshProfilesCall_0042bed4(void* value, void*)
{
    *(unsigned long*)value = 0;
}

extern "C" void __fastcall FableRefreshProfilesCall_0042cd84(
    void*,
    void*,
    void*)
{
}

extern "C" void __fastcall FableRefreshProfilesCall_0042abca(void*, void*)
{
}

extern "C" void* __fastcall FableRefreshProfilesCall_0041db1d(
    void*,
    void*,
    void*,
    bool)
{
    return &g_list;
}

extern "C" void __fastcall FableRefreshProfilesCall_00429c15(
    void* counted,
    void*,
    void* object)
{
    ((void**)counted)[0] = object;
    ((void**)counted)[1] = (void*)0x12340000;
}

extern "C" void __fastcall FableRefreshProfilesCall_0099eae0(void*, void*)
{
}

extern "C" void __fastcall FableRefreshProfilesCall_0059aa86(
    void* counted,
    void*,
    void* object,
    void* pointerInfo)
{
    ((void**)counted)[0] = object;
    ((void**)counted)[1] = pointerInfo;
}

extern "C" void __fastcall FableRefreshProfilesCall_0099b6a0(
    void* destination,
    void* source)
{
    *(void**)destination = source;
}

extern "C" void* __fastcall FableRefreshProfilesCall_00596a68(
    void*,
    void*,
    void* profile,
    float,
    void*,
    void*,
    bool)
{
    g_currentProfile = profile;
    return &g_buttons[g_buttonCalls++];
}

extern "C" void* __cdecl FableRefreshProfilesCall_00bfea1a(unsigned int bytes)
{
    if (bytes == 0x10)
        return &g_actions[g_actionAllocations++];
    if (bytes == 0x0c)
        return g_parameters[g_parameterAllocations++];
    return 0;
}

extern "C" void __fastcall FableRefreshProfilesCall_0042be50()
{
}

extern "C" void __fastcall FableRefreshProfilesCall_0042aa29(
    void* counted,
    void*,
    void* object)
{
    ((void**)counted)[0] = object;
    ((void**)counted)[1] = (void*)0x56780000;
}

extern "C" void* __cdecl FableRefreshProfilesCall_0040d2a0()
{
    return g_parameterName;
}

extern "C" void* __fastcall FableRefreshProfilesCall_004082d0(void*)
{
    return g_parameterName;
}

extern "C" unsigned char __fastcall FableRefreshProfilesCall_005957d9(
    void*,
    void*,
    void*)
{
    for (int i = 0; i != g_profileCount; ++i)
    {
        if (g_currentProfile == &g_profiles[i])
            return g_valid[i];
    }
    return 0;
}

extern "C" void* __fastcall FableRefreshProfilesCall_0054dae0(
    void* allocated,
    void*,
    void*)
{
    return allocated;
}

extern "C" void __fastcall FableRefreshProfilesCall_0042b646(
    void* slot,
    void*,
    void* parameter)
{
    *(void**)slot = *(void**)parameter;
}

extern "C" void __fastcall FableRefreshProfilesCall_00429160(void*, void*)
{
    ++g_releaseActionCalls;
}

extern "C" void __fastcall FableRefreshProfilesCall_004291de(void*, void*)
{
    ++g_releaseButtonCalls;
}

extern "C" void __fastcall FableRefreshProfilesCall_0059a994(void*, void*)
{
    ++g_releaseListCalls;
}

extern "C" void __fastcall FableRefreshProfilesCall_00412130(void*, void*)
{
    ++g_tidyCalls;
}

extern "C" void __fastcall FableRefreshProfilesCall_0099b510(void*, void*)
{
}

static void* __fastcall FindChild(void*, void*, void*)
{
    return g_oldList;
}

static void __fastcall ClearChild(void*, void*)
{
}

static void __fastcall Attach(void*, void*, void*)
{
    ++g_attachCalls;
}

static void __fastcall Append(void*, void*, void*)
{
    ++g_appendCalls;
}

static void __fastcall Finish(void*, void*)
{
    ++g_finishCalls;
}

static void __fastcall RootAdd(void*, void*, void*)
{
}

int main()
{
    g_vtable[0x0c / 4] = (void*)&FindChild;
    g_vtable[0xf0 / 4] = (void*)&Append;
    g_vtable[0xec / 4] = (void*)&RootAdd;
    g_vtable[0x108 / 4] = (void*)&Attach;
    g_vtable[0x11c / 4] = (void*)&Attach;
    g_vtable[0xac / 4] = (void*)&Finish;
    g_vtable[0xf8 / 4] = (void*)&ClearChild;

    Reset(0);
    if (g_manager.RefreshAvailableProfiles() != 0 ||
        g_profileEnumerationCalls != 1 ||
        g_buttonCalls != 1 ||
        g_actions[0].id != 0x125 ||
        g_attachCalls != 2 ||
        g_appendCalls != 1 ||
        g_finishCalls != 1 ||
        g_releaseActionCalls != 1 ||
        g_releaseButtonCalls != 2 ||
        g_releaseListCalls != 1 ||
        g_tidyCalls != 1)
    {
        std::printf("empty profile refresh failed\n");
        return 1;
    }

    Reset(3);
    if (g_manager.RefreshAvailableProfiles() != 3 ||
        g_buttonCalls != 4 ||
        g_actionAllocations != 4 ||
        g_parameterAllocations != 3 ||
        g_attachCalls != 8 ||
        g_appendCalls != 4 ||
        g_finishCalls != 1 ||
        g_releaseActionCalls != 4 ||
        g_releaseButtonCalls != 5 ||
        g_releaseListCalls != 1 ||
        g_tidyCalls != 1 ||
        g_actions[1].id != 0xdb ||
        g_actions[2].id != 0x124 ||
        g_actions[3].id != 0xdb)
    {
        std::printf("populated profile refresh failed\n");
        return 2;
    }

    std::printf("FSE2_00596CCE_TEST PASS\n");
    return 0;
}
