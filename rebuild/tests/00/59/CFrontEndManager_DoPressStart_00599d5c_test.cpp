// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::DoPressStart @ 0x00599D5C.

#include <cstdio>
#include <cstring>

extern "C" void __fastcall
CFrontEndManager_DoPressStart_00599d5c(void*, void*);

enum Event
{
    kNames = 1,
    kNewProfile,
    kGetService,
    kDefaultProfile,
    kVerify,
    kDestroyString,
    kLoadProfile,
    kResolve,
    kNext,
    kProfileMenu,
    kApply,
    kGetJoystick,
    kSetJoystick,
    kDestroyNames
};

struct ServiceProbe
{
    unsigned char bytes[0x10];
};

struct NamesProbe
{
    void* first;
    void* last;
    void* end;
};

struct InputProbe
{
    unsigned char bytes[0x140];
    long mainDevice;
};

static Event g_events[24];
static int g_eventCount;
static ServiceProbe g_service;
static InputProbe g_input;
static unsigned long g_profiles[3];
static long g_usedComponent;
static void* g_expectedManager;
static int g_profileCount;
static bool g_verifyResult;
static int g_newProfileCalls;
static void* g_defaultOutput;
static void* g_defaultProfile;
static long g_defaultFlags;
static void* g_verifyArgument;
static void* g_destroyedString;
static void* g_loadedProfile;
static unsigned long g_resolvedKey;
static void* g_nextComponent;
static long g_nextTransition;
static int g_profileMenuCalls;
static int g_applyCalls;
static long g_setDevice;
static void* g_namesAddress;
static void* g_destroyedNames;
static unsigned char g_joystickManager;

extern "C" void* FableFrontEndPressStartInputState = &g_input;

static void AddEvent(Event event)
{
    g_events[g_eventCount++] = event;
}

extern "C" void __fastcall
FableFrontEndPressStartGetProfileNames(
    void* manager,
    void*,
    void* names)
{
    AddEvent(kNames);
    g_namesAddress = names;
    NamesProbe* vector = (NamesProbe*)names;
    if (manager == g_expectedManager)
    {
        vector->first = g_profiles;
        vector->last = g_profiles + g_profileCount;
        vector->end = vector->last;
    }
}

extern "C" void __fastcall
FableFrontEndPressStartGotoNewProfile(void* manager, void*)
{
    AddEvent(kNewProfile);
    if (manager == g_expectedManager)
        ++g_newProfileCalls;
}

extern "C" void* __cdecl
FableFrontEndPressStartGetProfileService()
{
    AddEvent(kGetService);
    return &g_service;
}

extern "C" void* __fastcall
FableFrontEndPressStartGetDefaultProfile(
    void* service,
    void*,
    void* string,
    void* profile,
    long flags)
{
    AddEvent(kDefaultProfile);
    if (service == &g_service)
    {
        g_defaultOutput = string;
        g_defaultProfile = profile;
        g_defaultFlags = flags;
    }
    return string;
}

extern "C" unsigned char __fastcall
FableFrontEndPressStartVerifyProfile(
    void* manager,
    void*,
    void* defaultProfile)
{
    AddEvent(kVerify);
    if (manager == g_expectedManager)
        g_verifyArgument = defaultProfile;
    return g_verifyResult ? 1 : 0;
}

extern "C" void __fastcall
FableFrontEndPressStartDestroyString(void* string, void*)
{
    AddEvent(kDestroyString);
    g_destroyedString = string;
}

extern "C" void __fastcall
FableFrontEndPressStartLoadProfile(
    void* manager,
    void*,
    void* profile)
{
    AddEvent(kLoadProfile);
    if (manager == g_expectedManager)
        g_loadedProfile = profile;
}

extern "C" long** __fastcall
FableFrontEndPressStartResolveUsedKey(
    void* usedKeys,
    void*,
    const unsigned long* key)
{
    AddEvent(kResolve);
    if (usedKeys == (void*)((unsigned char*)g_expectedManager + 0x54))
        g_resolvedKey = *key;
    static long* result;
    result = &g_usedComponent;
    return &result;
}

extern "C" void __fastcall
FableFrontEndPressStartGotoNext(
    void* manager,
    void*,
    void* component,
    long transition)
{
    AddEvent(kNext);
    if (manager == g_expectedManager)
    {
        g_nextComponent = component;
        g_nextTransition = transition;
    }
}

extern "C" void __fastcall
FableFrontEndPressStartGotoProfileMenu(void* manager, void*)
{
    AddEvent(kProfileMenu);
    if (manager == g_expectedManager)
        ++g_profileMenuCalls;
}

extern "C" void __fastcall
FableFrontEndPressStartApplyProfileService(void* service, void*)
{
    AddEvent(kApply);
    if (service == &g_service)
        ++g_applyCalls;
}

extern "C" void* __cdecl
FableFrontEndPressStartGetJoystickManager()
{
    AddEvent(kGetJoystick);
    return &g_joystickManager;
}

extern "C" void __fastcall
FableFrontEndPressStartSetMainJoystick(
    void* joystickManager,
    void*,
    long device)
{
    AddEvent(kSetJoystick);
    if (joystickManager == &g_joystickManager)
        g_setDevice = device;
}

extern "C" void __fastcall
FableFrontEndPressStartDestroyProfileNames(void* names, void*)
{
    AddEvent(kDestroyNames);
    g_destroyedNames = names;
}

static void Reset(
    unsigned char* manager,
    int profileCount,
    bool verifyResult)
{
    std::memset(manager, 0, 0x80);
    std::memset(&g_service, 0, sizeof(g_service));
    g_eventCount = 0;
    g_expectedManager = manager;
    g_profileCount = profileCount;
    g_verifyResult = verifyResult;
    g_newProfileCalls = 0;
    g_defaultOutput = 0;
    g_defaultProfile = 0;
    g_defaultFlags = -1;
    g_verifyArgument = 0;
    g_destroyedString = 0;
    g_loadedProfile = 0;
    g_resolvedKey = 0xffffffff;
    g_nextComponent = 0;
    g_nextTransition = -1;
    g_profileMenuCalls = 0;
    g_applyCalls = 0;
    g_setDevice = -1;
    g_namesAddress = 0;
    g_destroyedNames = 0;
    g_input.mainDevice = 7;
    g_usedComponent = 0x44556677;
}

static bool EventsEqual(const Event* expected, unsigned long count)
{
    return g_eventCount == (int)count &&
        std::memcmp(g_events, expected, count * sizeof(Event)) == 0;
}

static bool RunEmpty()
{
    unsigned char manager[0x80];
    Reset(manager, 0, false);
    CFrontEndManager_DoPressStart_00599d5c(manager, 0);
    static const Event expected[] =
    {
        kNames, kNewProfile, kGetJoystick, kSetJoystick, kDestroyNames
    };
    return EventsEqual(expected, sizeof(expected) / sizeof(expected[0])) &&
        g_newProfileCalls == 1 &&
        g_setDevice == 7 &&
        g_destroyedNames == g_namesAddress;
}

static bool RunSingle(bool verifyResult)
{
    unsigned char manager[0x80];
    Reset(manager, 1, verifyResult);
    CFrontEndManager_DoPressStart_00599d5c(manager, 0);
    static const Event success[] =
    {
        kNames, kGetService, kDefaultProfile, kVerify, kDestroyString,
        kLoadProfile, kResolve, kNext, kGetJoystick, kSetJoystick,
        kDestroyNames
    };
    static const Event failure[] =
    {
        kNames, kGetService, kDefaultProfile, kVerify, kDestroyString,
        kProfileMenu, kGetService, kGetService, kApply,
        kGetJoystick, kSetJoystick, kDestroyNames
    };
    const bool route = verifyResult
        ? EventsEqual(success, sizeof(success) / sizeof(success[0]))
        : EventsEqual(failure, sizeof(failure) / sizeof(failure[0]));
    return route &&
        g_defaultProfile == g_profiles &&
        g_defaultFlags == 0 &&
        g_verifyArgument == g_defaultOutput &&
        g_destroyedString == g_defaultOutput &&
        g_setDevice == 7 &&
        g_destroyedNames == g_namesAddress &&
        (verifyResult
            ? g_loadedProfile == g_profiles &&
              g_resolvedKey == 0 &&
              g_nextComponent == &g_usedComponent &&
              g_nextTransition == 1
            : g_profileMenuCalls == 1 &&
              g_service.bytes[0x0c] == 1 &&
              g_applyCalls == 1);
}

static bool RunMultiple()
{
    unsigned char manager[0x80];
    Reset(manager, 2, true);
    CFrontEndManager_DoPressStart_00599d5c(manager, 0);
    static const Event expected[] =
    {
        kNames, kProfileMenu, kGetService, kGetService, kApply,
        kGetJoystick, kSetJoystick, kDestroyNames
    };
    return EventsEqual(expected, sizeof(expected) / sizeof(expected[0])) &&
        g_profileMenuCalls == 1 &&
        g_service.bytes[0x0c] == 1 &&
        g_applyCalls == 1 &&
        g_destroyedString == 0 &&
        g_setDevice == 7 &&
        g_destroyedNames == g_namesAddress;
}

int main()
{
    const bool empty = RunEmpty();
    const bool success = RunSingle(true);
    const bool failure = RunSingle(false);
    const bool multiple = RunMultiple();
    if (!empty || !success || !failure || !multiple)
    {
        std::printf(
            "FSE2_00599d5c_TEST FAIL empty=%d success=%d "
            "failure=%d multiple=%d\n",
            empty ? 1 : 0,
            success ? 1 : 0,
            failure ? 1 : 0,
            multiple ? 1 : 0);
        return 1;
    }
    std::puts("FSE2_00599d5c_TEST PASS");
    return 0;
}
