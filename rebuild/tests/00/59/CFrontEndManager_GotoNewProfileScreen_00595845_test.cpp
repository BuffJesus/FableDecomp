// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::GotoNewProfileScreen @ 0x00595845.

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

struct Definition
{
    void** vtable;
    int references;
    unsigned char payload[0x1b0];
};

struct Service
{
    void** vtable;
};

struct ProfileManager
{
    unsigned char bytes[0x20];
};

struct FrontEndManagerProbe
{
    unsigned char bytes[0xa4];
};

extern "C" void __fastcall
CFrontEndManager_GotoNewProfileScreen_00595845(
    void* manager,
    void*);

enum Event
{
    kGetNames = 1,
    kGetDefinition,
    kPlaySound,
    kRelease,
    kTop,
    kState,
    kOwnership,
    kDestroyNames
};

static Event g_events[16];
static int g_eventCount;
static void* g_expectedManager;
static void* g_expectedStack;
static Component* g_topComponent;
static void* g_namesAddress;
static void* g_destroyedNamesAddress;
static unsigned long g_destroyedNames[3];
static int g_getProfileManagerCalls;
static int g_getManagerCalls;
static int g_getNamesCalls;
static int g_destroyNamesCalls;
static int g_stackTopCalls;
static void* g_ownershipAddress;
static int g_getDefinitionCalls;
static int g_playCalls;
static void* g_playCriteria;
static int g_playArgument;
static int g_releaseCalls;

static void* g_componentVtable[0xc4 / 4];
static void* g_serviceVtable[0xac / 4];
static void* g_definitionVtable[2];
static ProfileManager g_profileManager;
static Service g_service;
static Definition g_definition;

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

extern "C" void* __cdecl
FableFrontEndGotoNewProfileGetProfileManager()
{
    ++g_getProfileManagerCalls;
    return &g_profileManager;
}

extern "C" void __fastcall
FableFrontEndGotoNewProfileGetNames(
    void* manager,
    void*,
    void* names)
{
    AddEvent(kGetNames);
    ++g_getNamesCalls;
    g_namesAddress = names;
    unsigned long* vector = (unsigned long*)names;
    if (manager != g_expectedManager ||
        vector[0] != 0 ||
        vector[1] != 0 ||
        vector[2] != 0)
    {
        vector[0] = 0xffffffff;
        return;
    }
    vector[0] = 0x11111111;
    vector[1] = 0x22222222;
    vector[2] = 0x33333333;
}

extern "C" void* __cdecl
FableFrontEndGotoNewProfileGetManager()
{
    ++g_getManagerCalls;
    return &g_service;
}

static Definition** __fastcall
ServiceGetDefinition(void*, void*, Definition** result)
{
    AddEvent(kGetDefinition);
    ++g_getDefinitionCalls;
    *result = &g_definition;
    return result;
}

static void __fastcall
ServicePlaySound(void*, void*, void* criteria, int argument)
{
    AddEvent(kPlaySound);
    ++g_playCalls;
    g_playCriteria = criteria;
    g_playArgument = argument;
}

static void __fastcall DefinitionRelease(void*, void*)
{
    AddEvent(kRelease);
    ++g_releaseCalls;
}

extern "C" void** __fastcall
FableFrontEndGotoNewProfileStackTop(void* stack, void*)
{
    AddEvent(kTop);
    ++g_stackTopCalls;
    if (stack != g_expectedStack)
        return 0;
    return (void**)&g_topComponent;
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

extern "C" void __fastcall
FableFrontEndGotoNewProfileDestroyNames(void* names, void*)
{
    AddEvent(kDestroyNames);
    ++g_destroyNamesCalls;
    g_destroyedNamesAddress = names;
    std::memcpy(g_destroyedNames, names, sizeof(g_destroyedNames));
}

static void Reset(
    FrontEndManagerProbe& manager,
    Component& component,
    int references)
{
    std::memset(&manager, 0, sizeof(manager));
    std::memset(&component, 0, sizeof(component));
    std::memset(&g_profileManager, 0, sizeof(g_profileManager));
    std::memset(&g_definition, 0, sizeof(g_definition));
    component.vtable = g_componentVtable;
    component.ownershipToken = 0xaabbccdd;
    component.state = -1;
    g_definition.vtable = g_definitionVtable;
    g_definition.references = references;
    g_topComponent = &component;
    g_expectedManager = &manager;
    g_expectedStack = (void*)((char*)&manager + 0x20);
    ManagerPointer(manager, 0x60) = (void*)0x60606060;
    ManagerPointer(manager, 0x98) = (void*)0x98989898;
    ManagerPointer(manager, 0x9c) = (void*)0x9c9c9c9c;

    g_eventCount = 0;
    g_namesAddress = 0;
    g_destroyedNamesAddress = 0;
    std::memset(g_destroyedNames, 0, sizeof(g_destroyedNames));
    g_getProfileManagerCalls = 0;
    g_getManagerCalls = 0;
    g_getNamesCalls = 0;
    g_destroyNamesCalls = 0;
    g_stackTopCalls = 0;
    g_ownershipAddress = 0;
    g_getDefinitionCalls = 0;
    g_playCalls = 0;
    g_playCriteria = 0;
    g_playArgument = 0;
    g_releaseCalls = 0;
}

static int RunCase(int references)
{
    FrontEndManagerProbe manager;
    Component component;
    Reset(manager, component, references);

    CFrontEndManager_GotoNewProfileScreen_00595845(
        &manager,
        0);

    if (g_getProfileManagerCalls != 1 ||
        g_profileManager.bytes[0x0c] != 1 ||
        manager.bytes[0x64] != 1 ||
        manager.bytes[0xa0] != 1)
        return 1;
    if (g_getNamesCalls != 1 ||
        g_destroyNamesCalls != 1 ||
        g_namesAddress == 0 ||
        g_destroyedNamesAddress != g_namesAddress ||
        g_destroyedNames[0] != 0x11111111 ||
        g_destroyedNames[1] != 0x22222222 ||
        g_destroyedNames[2] != 0x33333333)
        return 2;
    if (g_getManagerCalls != 3 ||
        g_getDefinitionCalls != 1 ||
        g_playCalls != 1 ||
        g_playCriteria !=
            (void*)((char*)&g_definition + 0x1a0) ||
        g_playArgument != 1)
        return 3;
    if (component.stateCalls != 1 ||
        component.state != 6 ||
        g_stackTopCalls != 1 ||
        g_ownershipAddress != (void*)((char*)&component + 4))
        return 4;
    if (ManagerPointer(manager, 0x60) != (void*)0x60606060 ||
        ManagerPointer(manager, 0x98) != (void*)0x98989898 ||
        ManagerPointer(manager, 0x9c) != (void*)0x9c9c9c9c)
        return 5;
    if (references == 1)
    {
        if (g_definition.references != 0 || g_releaseCalls != 1)
            return 6;
    }
    else if (g_definition.references != references - 1 ||
             g_releaseCalls != 0)
    {
        return 7;
    }
    if (g_events[g_eventCount - 1] != kDestroyNames)
        return 8;
    return 0;
}

int main()
{
    std::memset(g_componentVtable, 0, sizeof(g_componentVtable));
    std::memset(g_serviceVtable, 0, sizeof(g_serviceVtable));
    std::memset(g_definitionVtable, 0, sizeof(g_definitionVtable));
    g_componentVtable[0xc0 / 4] = (void*)&ComponentSetState;
    g_serviceVtable[0x14 / 4] = (void*)&ServiceTakeOwnership;
    g_serviceVtable[0xa0 / 4] = (void*)&ServicePlaySound;
    g_serviceVtable[0xa8 / 4] = (void*)&ServiceGetDefinition;
    g_definitionVtable[1] = (void*)&DefinitionRelease;
    g_service.vtable = g_serviceVtable;

    int failure = RunCase(1);
    if (failure != 0)
        return failure;
    failure = RunCase(2);
    if (failure != 0)
        return 20 + failure;

    std::printf("FSE2_00595845_TEST PASS\n");
    return 0;
}
