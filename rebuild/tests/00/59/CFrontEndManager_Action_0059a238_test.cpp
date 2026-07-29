// Focused startup/frontend dispatch fixture for
// NUISystem::CFrontEndManager::Action @ 0x0059A238.

#include <cstdio>
#include <cstring>

struct CFrontEndAction
{
    long id;
    long payloadTag;
};

struct CFrontEndActionPointer
{
    CFrontEndAction* pointer;
};

struct ComponentProbe
{
    void** vtable;
    unsigned char bytes[0x30];
};

struct ManagerProbe
{
    unsigned char bytes[0xe0];
};

extern "C" void __fastcall
CFrontEndManager_Action_0059a238(
    void* manager,
    void*,
    CFrontEndActionPointer* action);

enum Event
{
    kPause = 1,
    kUnpause,
    kResolve,
    kNext,
    kPrevious,
    kPressStart,
    kProfile,
    kQuit,
    kInvalidSave,
    kRefreshSaves,
    kNoSaves,
    kRefreshDelete,
    kLoadProfile,
    kNewProfile,
    kKeyboardConfirm,
    kKeyboardCancel,
    kScoreboard,
    kComponentQuit
};

static Event g_events[16];
static int g_eventCount;
static double g_now;
static int g_installerActive;
static unsigned long g_resolvedKey;
static void* g_resolvedComponent;
static void* g_nextComponent;
static int g_nextForce;
static int g_previousForce;
static CFrontEndAction* g_helperAction;
static void* g_helperObject;
static long g_refreshSavedGamesResult;
static long g_refreshDeleteResult;
static void* g_componentVtable[5];

static void AddEvent(Event event)
{
    g_events[g_eventCount++] = event;
}

static unsigned char& ManagerByte(
    ManagerProbe& manager,
    unsigned long offset)
{
    return manager.bytes[offset];
}

static unsigned long& ManagerUnsigned(
    ManagerProbe& manager,
    unsigned long offset)
{
    return *(unsigned long*)&manager.bytes[offset];
}

static void*& ManagerPointer(
    ManagerProbe& manager,
    unsigned long offset)
{
    return *(void**)&manager.bytes[offset];
}

static double ManagerTime(const ManagerProbe& manager)
{
    return *(const double*)&manager.bytes[0xd0];
}

extern "C" double __cdecl FableFrontEndActionGetTime()
{
    return g_now;
}

extern "C" int __cdecl FableFrontEndActionInstallerActive()
{
    return g_installerActive;
}

extern "C" void __cdecl FableFrontEndActionPauseInstaller()
{
    AddEvent(kPause);
}

extern "C" void __cdecl FableFrontEndActionUnpauseInstaller()
{
    AddEvent(kUnpause);
}

extern "C" void* __fastcall
FableFrontEndActionResolveUsedKey(void*, void*, unsigned long key)
{
    AddEvent(kResolve);
    g_resolvedKey = key;
    return g_resolvedComponent;
}

extern "C" void __fastcall
FableFrontEndActionGotoNext(
    void*,
    void*,
    void* component,
    int force)
{
    AddEvent(kNext);
    g_nextComponent = component;
    g_nextForce = force;
}

extern "C" void __fastcall
FableFrontEndActionGotoPrevious(void*, void*, int force)
{
    AddEvent(kPrevious);
    g_previousForce = force;
}

extern "C" void __fastcall
FableFrontEndActionDoPressStart(void*, void*)
{
    AddEvent(kPressStart);
}

extern "C" void __fastcall
FableFrontEndActionGotoProfileMenu(void*, void*)
{
    AddEvent(kProfile);
}

extern "C" void __fastcall
FableFrontEndActionRequestQuit(void*, void*)
{
    AddEvent(kQuit);
}

extern "C" void __fastcall
FableFrontEndActionShowInvalidSave(
    void*,
    void*,
    CFrontEndAction* action)
{
    AddEvent(kInvalidSave);
    g_helperAction = action;
}

extern "C" long __fastcall
FableFrontEndActionRefreshSavedGames(void*, void*)
{
    AddEvent(kRefreshSaves);
    return g_refreshSavedGamesResult;
}

extern "C" void __fastcall
FableFrontEndActionNoSavedGameFallback(void*, void*)
{
    AddEvent(kNoSaves);
}

extern "C" long __fastcall
FableFrontEndActionRefreshProfilesForDelete(void*, void*)
{
    AddEvent(kRefreshDelete);
    return g_refreshDeleteResult;
}

extern "C" void __fastcall
FableFrontEndActionLoadProfile(
    void*,
    void*,
    CFrontEndAction* action)
{
    AddEvent(kLoadProfile);
    g_helperAction = action;
}

extern "C" void __fastcall
FableFrontEndActionBeginNewProfile(void*, void*)
{
    AddEvent(kNewProfile);
}

extern "C" void __fastcall
FableFrontEndActionKeyboardConfirm(void* keyboard, void*)
{
    AddEvent(kKeyboardConfirm);
    g_helperObject = keyboard;
}

extern "C" void __fastcall
FableFrontEndActionKeyboardCancel(void* keyboard, void*)
{
    AddEvent(kKeyboardCancel);
    g_helperObject = keyboard;
}

extern "C" void __fastcall
FableFrontEndActionSetScoreboardOptions(void*, void*)
{
    AddEvent(kScoreboard);
}

static void __fastcall ComponentQuit(void*, void*)
{
    AddEvent(kComponentQuit);
}

static void Reset(ManagerProbe& manager)
{
    std::memset(&manager, 0, sizeof(manager));
    g_eventCount = 0;
    g_now = 41.25;
    g_installerActive = 0;
    g_resolvedKey = 0xffffffff;
    g_resolvedComponent = (void*)0x12345678;
    g_nextComponent = 0;
    g_nextForce = -1;
    g_previousForce = -1;
    g_helperAction = 0;
    g_helperObject = 0;
    g_refreshSavedGamesResult = 1;
    g_refreshDeleteResult = 0;
}

static void Dispatch(
    ManagerProbe& manager,
    CFrontEndAction& action,
    long id)
{
    action.id = id;
    CFrontEndActionPointer pointer;
    pointer.pointer = &action;
    CFrontEndManager_Action_0059a238(&manager, 0, &pointer);
}

static int CheckForwardRoutes()
{
    struct Route
    {
        long action;
        unsigned long key;
    };
    static const Route routes[] =
    {
        { 9, 0x01 },
        { 11, 0x03 },
        { 12, 0x04 },
        { 13, 0x05 },
        { 14, 0x06 },
        { 67, 0x09 },
        { 219, 0x0f },
        { 283, 0x16 },
        { 297, 0x18 },
        { 314, 0x1a },
        { 321, 0x1c },
        { 3025, 0x14 }
    };

    for (unsigned int i = 0; i != sizeof(routes) / sizeof(routes[0]); ++i)
    {
        ManagerProbe manager;
        Reset(manager);
        CFrontEndAction action;
        Dispatch(manager, action, routes[i].action);
        if (ManagerTime(manager) != g_now ||
            g_eventCount != 2 ||
            g_events[0] != kResolve ||
            g_events[1] != kNext ||
            g_resolvedKey != routes[i].key ||
            g_nextComponent != g_resolvedComponent ||
            g_nextForce != 0)
            return 10 + (int)i;
    }
    return 0;
}

static int CheckEntryAndCoreActions()
{
    ManagerProbe manager;
    Reset(manager);
    g_installerActive = 1;
    CFrontEndAction action;
    Dispatch(manager, action, 297);
    if (g_eventCount != 3 ||
        g_events[0] != kPause ||
        g_events[1] != kResolve ||
        g_events[2] != kNext ||
        ManagerByte(manager, 0xd8) != 1 ||
        ManagerByte(manager, 0xd9) != 0)
        return 30;

    g_eventCount = 0;
    g_now = 99.5;
    Dispatch(manager, action, 86);
    if (ManagerTime(manager) != 99.5 ||
        g_eventCount != 1 ||
        g_events[0] != kPrevious ||
        g_previousForce != 0)
        return 31;

    g_eventCount = 0;
    Dispatch(manager, action, 229);
    if (g_eventCount != 1 || g_events[0] != kPressStart)
        return 32;

    g_eventCount = 0;
    Dispatch(manager, action, 296);
    if (g_eventCount != 1 || g_events[0] != kQuit)
        return 33;

    g_eventCount = 0;
    Dispatch(manager, action, 0x777);
    if (g_eventCount != 0 || ManagerTime(manager) != g_now)
        return 34;
    return 0;
}

static int CheckInvalidSaveAndConditionalErrors()
{
    ManagerProbe manager;
    Reset(manager);
    CFrontEndAction action;
    action.payloadTag = 0x55aa;
    Dispatch(manager, action, 220);
    if (g_eventCount != 3 ||
        g_events[0] != kInvalidSave ||
        g_events[1] != kResolve ||
        g_events[2] != kNext ||
        g_helperAction != &action ||
        g_resolvedKey != 0x10)
        return 40;

    Reset(manager);
    g_refreshSavedGamesResult = 0;
    ManagerUnsigned(manager, 0xbc) = 0;
    Dispatch(manager, action, 66);
    if (g_eventCount != 2 ||
        g_events[0] != kRefreshSaves ||
        g_events[1] != kNoSaves)
        return 41;

    Reset(manager);
    g_refreshSavedGamesResult = 0;
    ManagerUnsigned(manager, 0xbc) = 1;
    Dispatch(manager, action, 66);
    if (g_eventCount != 3 ||
        g_events[0] != kRefreshSaves ||
        g_events[1] != kResolve ||
        g_events[2] != kNext ||
        g_resolvedKey != 0x08)
        return 42;

    Reset(manager);
    g_refreshDeleteResult = 0;
    Dispatch(manager, action, 250);
    if (g_resolvedKey != 0x0c)
        return 43;

    Reset(manager);
    g_refreshDeleteResult = 3;
    Dispatch(manager, action, 250);
    if (g_resolvedKey != 0x15)
        return 44;
    return 0;
}

static int CheckForcedLoadAndComponentQuit()
{
    ManagerProbe manager;
    Reset(manager);
    CFrontEndAction action;
    action.payloadTag = 0x1122;
    Dispatch(manager, action, 292);
    if (g_eventCount != 3 ||
        g_events[0] != kLoadProfile ||
        g_events[1] != kResolve ||
        g_events[2] != kNext ||
        g_helperAction != &action ||
        g_resolvedKey != 0 ||
        g_nextForce != 1)
        return 50;

    ComponentProbe component;
    std::memset(&component, 0, sizeof(component));
    std::memset(g_componentVtable, 0, sizeof(g_componentVtable));
    g_componentVtable[0x10 / 4] = (void*)&ComponentQuit;
    component.vtable = g_componentVtable;

    Reset(manager);
    g_installerActive = 1;
    ManagerByte(manager, 0xd8) = 1;
    ManagerPointer(manager, 0x1c) = &component;
    Dispatch(manager, action, 15);
    if (g_eventCount != 2 ||
        g_events[0] != kUnpause ||
        g_events[1] != kComponentQuit ||
        ManagerByte(manager, 0xd8) != 0 ||
        ManagerByte(manager, 0xd9) != 1 ||
        *((unsigned char*)&component + 0x29) != 1)
        return 51;
    return 0;
}

static int CheckKeyboardNullAndDispatch()
{
    ManagerProbe manager;
    Reset(manager);
    CFrontEndAction action;

    Dispatch(manager, action, 294);
    if (g_eventCount != 0)
        return 60;
    Dispatch(manager, action, 295);
    if (g_eventCount != 0)
        return 61;

    void* keyboard = (void*)0x76543210;
    ManagerPointer(manager, 0x60) = keyboard;
    Dispatch(manager, action, 294);
    if (g_eventCount != 1 ||
        g_events[0] != kKeyboardConfirm ||
        g_helperObject != keyboard)
        return 62;

    g_eventCount = 0;
    g_helperObject = 0;
    Dispatch(manager, action, 295);
    if (g_eventCount != 1 ||
        g_events[0] != kKeyboardCancel ||
        g_helperObject != keyboard)
        return 63;
    return 0;
}

int main()
{
    int failure = CheckForwardRoutes();
    if (failure != 0)
        return failure;
    failure = CheckEntryAndCoreActions();
    if (failure != 0)
        return failure;
    failure = CheckInvalidSaveAndConditionalErrors();
    if (failure != 0)
        return failure;
    failure = CheckForcedLoadAndComponentQuit();
    if (failure != 0)
        return failure;
    failure = CheckKeyboardNullAndDispatch();
    if (failure != 0)
        return failure;

    std::printf("FSE2_0059a238_TEST PASS\n");
    return 0;
}
