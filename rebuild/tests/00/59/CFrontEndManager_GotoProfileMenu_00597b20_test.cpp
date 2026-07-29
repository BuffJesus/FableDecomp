// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::GotoProfileMenu @ 0x00597B20.

#include <cstdio>
#include <cstring>

struct Component
{
    void** vtable;
    unsigned long ownershipToken;
    unsigned char padding[0x30];
    int state;
    int stateCalls;
};

struct Service
{
    void** vtable;
};

struct FrontEndManagerProbe
{
    unsigned char bytes[0xa0];
};

extern "C" void __fastcall
CFrontEndManager_GotoProfileMenu_00597b20(
    void* manager,
    void*);

enum Event
{
    kRefresh = 1,
    kSize,
    kResolve,
    kTop,
    kState,
    kOwnership,
    kPop,
    kPush,
    kNext
};

static Event g_events[32];
static int g_eventCount;
static void* g_stack[8];
static int g_stackCount;
static void* g_expectedManager;
static void* g_expectedStack;
static void* g_expectedUsedKeys;
static long* g_usedKeys[0x10];
static long g_refreshResult;
static unsigned long g_resolvedKeys[4];
static int g_resolveCount;
static int g_topCalls;
static int g_sizeCalls;
static int g_popCalls;
static int g_pushCalls;
static void** g_pushAddress;
static void* g_pushedComponent;
static void* g_ownershipAddress;
static int g_getManagerCalls;
static int g_nextCalls;
static void* g_nextComponent;
static unsigned char g_nextTransition;

static void* g_componentVtable[0xc4 / 4];
static void* g_serviceVtable[0x18 / 4];
static Service g_service;

static void AddEvent(Event event)
{
    g_events[g_eventCount++] = event;
}

static void*& ManagerPointer(
    FrontEndManagerProbe& manager,
    unsigned long offset)
{
    return *(void**)&manager.bytes[offset];
}

extern "C" long __fastcall
FableFrontEndGotoProfileRefreshProfiles(void* manager, void*)
{
    AddEvent(kRefresh);
    return manager == g_expectedManager ? g_refreshResult : -1;
}

extern "C" unsigned long __fastcall
FableFrontEndGotoProfileStackSize(
    void* stackEnd,
    void*,
    void* stack)
{
    AddEvent(kSize);
    ++g_sizeCalls;
    if (stack != g_expectedStack ||
        stackEnd != (void*)((char*)g_expectedStack + 0x10))
        return 0;
    return g_stackCount;
}

extern "C" long** __fastcall
FableFrontEndGotoProfileResolveUsedKey(
    void* usedKeys,
    void*,
    const unsigned long* key)
{
    AddEvent(kResolve);
    const unsigned long resolved =
        usedKeys == g_expectedUsedKeys ? *key : 0xffffffff;
    g_resolvedKeys[g_resolveCount++] = resolved;
    return &g_usedKeys[resolved & 0x0f];
}

extern "C" void** __fastcall
FableFrontEndGotoProfileStackTop(void* stack, void*)
{
    AddEvent(kTop);
    ++g_topCalls;
    if (stack != g_expectedStack || g_stackCount == 0)
        return 0;
    return &g_stack[g_stackCount - 1];
}

static void __fastcall
ComponentSetState(void* object, void*, int state)
{
    AddEvent(kState);
    Component* component = (Component*)object;
    component->state = state;
    ++component->stateCalls;
}

static void __fastcall
ServiceTakeOwnership(void*, void*, void* ownershipAddress)
{
    AddEvent(kOwnership);
    g_ownershipAddress = ownershipAddress;
}

extern "C" void* __cdecl
FableFrontEndGotoProfileGetManager()
{
    ++g_getManagerCalls;
    return &g_service;
}

extern "C" void __fastcall
FableFrontEndGotoProfileStackPop(void* stack, void*)
{
    AddEvent(kPop);
    ++g_popCalls;
    if (stack == g_expectedStack && g_stackCount != 0)
        --g_stackCount;
}

extern "C" void __fastcall
FableFrontEndGotoProfileStackPush(
    void* stack,
    void*,
    void** component)
{
    AddEvent(kPush);
    ++g_pushCalls;
    g_pushAddress = component;
    g_pushedComponent = *component;
    if (stack == g_expectedStack)
        g_stack[g_stackCount++] = *component;
}

extern "C" void __fastcall
FableFrontEndGotoProfileNext(
    void* manager,
    void*,
    void* component,
    unsigned char transition)
{
    AddEvent(kNext);
    if (manager == g_expectedManager)
    {
        ++g_nextCalls;
        g_nextComponent = component;
        g_nextTransition = transition;
    }
}

static void Reset(FrontEndManagerProbe& manager)
{
    std::memset(&manager, 0, sizeof(manager));
    std::memset(g_stack, 0, sizeof(g_stack));
    std::memset(g_usedKeys, 0, sizeof(g_usedKeys));
    g_eventCount = 0;
    g_stackCount = 0;
    g_expectedManager = &manager;
    g_expectedStack = (void*)((char*)&manager + 0x20);
    g_expectedUsedKeys = (void*)((char*)&manager + 0x54);
    g_refreshResult = 1;
    g_resolveCount = 0;
    g_topCalls = 0;
    g_sizeCalls = 0;
    g_popCalls = 0;
    g_pushCalls = 0;
    g_pushAddress = 0;
    g_pushedComponent = 0;
    g_ownershipAddress = 0;
    g_getManagerCalls = 0;
    g_nextCalls = 0;
    g_nextComponent = 0;
    g_nextTransition = 0xff;
}

static void InitialiseComponent(Component& component)
{
    std::memset(&component, 0, sizeof(component));
    component.vtable = g_componentVtable;
    component.ownershipToken = 0x12345678;
    component.state = -1;
}

static int CheckDirectReplacement(long profileCount)
{
    FrontEndManagerProbe manager;
    Reset(manager);
    Component previous;
    Component profileList;
    Component profilesAvailable;
    Component noProfiles;
    InitialiseComponent(previous);
    InitialiseComponent(profileList);
    InitialiseComponent(profilesAvailable);
    InitialiseComponent(noProfiles);
    g_refreshResult = profileCount;
    g_stack[0] = &previous;
    g_stack[1] = &profileList;
    g_stackCount = 2;
    g_usedKeys[0x0b] = (long*)&profileList;
    g_usedKeys[0x07] = (long*)&profilesAvailable;
    g_usedKeys[0x0c] = (long*)&noProfiles;

    CFrontEndManager_GotoProfileMenu_00597b20(&manager, 0);

    Component* expected =
        profileCount == 0 ? &noProfiles : &profilesAvailable;
    unsigned long expectedKey =
        profileCount == 0 ? 0x0c : 0x07;
    if (g_resolveCount != 2 ||
        g_resolvedKeys[0] != 0x0b ||
        g_resolvedKeys[1] != expectedKey)
        return 1;
    if (profileList.stateCalls != 1 ||
        profileList.state != 6 ||
        g_ownershipAddress != (void*)((char*)&profileList + 4))
        return 2;
    if (g_popCalls != 1 ||
        g_pushCalls != 1 ||
        g_pushAddress !=
            (void**)((char*)&manager + 0x9c) ||
        g_pushedComponent != expected ||
        g_stackCount != 2 ||
        g_stack[0] != &previous ||
        g_stack[1] != expected)
        return 3;
    if (ManagerPointer(manager, 0x98) != &profileList ||
        ManagerPointer(manager, 0x9c) != expected ||
        g_nextCalls != 0)
        return 4;
    return 0;
}

static int CheckDelegatedRoute(bool shallow)
{
    FrontEndManagerProbe manager;
    Reset(manager);
    Component root;
    Component current;
    Component other;
    Component profiles;
    InitialiseComponent(root);
    InitialiseComponent(current);
    InitialiseComponent(other);
    InitialiseComponent(profiles);
    g_stack[0] = &root;
    g_stackCount = 1;
    if (!shallow)
    {
        g_stack[1] = &current;
        g_stackCount = 2;
    }
    g_usedKeys[0x0b] = (long*)&other;
    g_usedKeys[0x07] = (long*)&profiles;

    CFrontEndManager_GotoProfileMenu_00597b20(&manager, 0);

    if (g_nextCalls != 1 ||
        g_nextComponent != &profiles ||
        g_nextTransition != 0 ||
        g_popCalls != 0 ||
        g_pushCalls != 0 ||
        g_getManagerCalls != 0)
        return shallow ? 10 : 11;
    if (g_resolvedKeys[g_resolveCount - 1] != 0x07)
        return shallow ? 12 : 13;
    return 0;
}

int main()
{
    std::memset(g_componentVtable, 0, sizeof(g_componentVtable));
    std::memset(g_serviceVtable, 0, sizeof(g_serviceVtable));
    g_componentVtable[0xc0 / 4] = (void*)&ComponentSetState;
    g_serviceVtable[0x14 / 4] = (void*)&ServiceTakeOwnership;
    g_service.vtable = g_serviceVtable;

    int failure = CheckDirectReplacement(3);
    if (failure != 0)
        return failure;
    failure = CheckDirectReplacement(0);
    if (failure != 0)
        return 20 + failure;
    failure = CheckDelegatedRoute(true);
    if (failure != 0)
        return failure;
    failure = CheckDelegatedRoute(false);
    if (failure != 0)
        return failure;

    std::printf("FSE2_00597b20_TEST PASS\n");
    return 0;
}
