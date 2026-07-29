// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::SetMainMenuDefName @ 0x00595A06.

#include <cstdio>
#include <cstring>

extern "C" void __fastcall
CFrontEndManager_SetMainMenuDefName_00595a06(
    void* manager,
    void*,
    const void* definition);

struct ComponentProbe
{
    void** vtable;
    int deleteCalls;
    long deleteFlag;
    int definitionCalls;
    const void* requestedDefinition;
    int activateCalls;
};

enum Event
{
    kResolve = 1,
    kGetDefinition,
    kCompare,
    kDestroyTemporary,
    kDelete,
    kGetManager,
    kCreate,
    kActivate
};

static Event g_events[24];
static int g_eventCount;
static void* g_expectedUsedKeys;
static void* g_mapValue;
static unsigned long g_keys[8];
static int g_resolveCalls;
static unsigned char g_differs;
static unsigned char g_comparison;
static void* g_temporaryAddress;
static void* g_destroyedTemporary;
static unsigned char g_service;
static void* g_createdComponent;
static const void* g_createdDefinition;
static long g_createdFrontEnd;
static int g_createCalls;
static void* g_vtable[0x154 / 4];
static ComponentProbe g_oldComponent;
static ComponentProbe g_newComponent;

static void AddEvent(Event event)
{
    g_events[g_eventCount++] = event;
}

extern "C" void** __fastcall
FableFrontEndMainMenuResolveUsedKey(
    void* usedKeys,
    void*,
    const unsigned long* key)
{
    AddEvent(kResolve);
    if (usedKeys == g_expectedUsedKeys)
        g_keys[g_resolveCalls] = *key;
    ++g_resolveCalls;
    return &g_mapValue;
}

static void* __fastcall
ComponentGetDefinition(
    void* object,
    void*,
    void* temporary,
    const void* requested)
{
    AddEvent(kGetDefinition);
    ComponentProbe* component = (ComponentProbe*)object;
    ++component->definitionCalls;
    component->requestedDefinition = requested;
    g_temporaryAddress = temporary;
    *(void**)temporary = &g_comparison;
    return &g_comparison;
}

extern "C" unsigned char __fastcall
FableFrontEndMainMenuDefinitionDiffers(void* comparison, void*)
{
    AddEvent(kCompare);
    return comparison == &g_comparison ? g_differs : 0;
}

extern "C" void __fastcall
FableFrontEndMainMenuDestroyTemporary(void* temporary, void*)
{
    AddEvent(kDestroyTemporary);
    g_destroyedTemporary = temporary;
}

static void __fastcall
ComponentDelete(void* object, void*, long flag)
{
    AddEvent(kDelete);
    ComponentProbe* component = (ComponentProbe*)object;
    ++component->deleteCalls;
    component->deleteFlag = flag;
}

static void __fastcall
ComponentActivate(void* object, void*)
{
    AddEvent(kActivate);
    ++((ComponentProbe*)object)->activateCalls;
}

extern "C" void* __cdecl
FableFrontEndMainMenuGetManager()
{
    AddEvent(kGetManager);
    return &g_service;
}

extern "C" void* __fastcall
FableFrontEndMainMenuCreateComponent(
    void* manager,
    void*,
    const void* definition,
    long frontEnd)
{
    AddEvent(kCreate);
    if (manager == &g_service)
    {
        ++g_createCalls;
        g_createdDefinition = definition;
        g_createdFrontEnd = frontEnd;
    }
    return g_createdComponent;
}

static void InitialiseComponent(ComponentProbe& component)
{
    std::memset(&component, 0, sizeof(component));
    component.vtable = g_vtable;
    component.deleteFlag = -1;
}

static void Reset(unsigned char* manager, bool existing, bool differs)
{
    std::memset(manager, 0, 0x80);
    std::memset(g_vtable, 0, sizeof(g_vtable));
    g_vtable[0] = (void*)ComponentDelete;
    g_vtable[0xac / 4] = (void*)ComponentActivate;
    g_vtable[0x150 / 4] = (void*)ComponentGetDefinition;
    InitialiseComponent(g_oldComponent);
    InitialiseComponent(g_newComponent);
    g_eventCount = 0;
    g_expectedUsedKeys = manager + 0x54;
    g_mapValue = existing ? &g_oldComponent : 0;
    std::memset(g_keys, 0xff, sizeof(g_keys));
    g_resolveCalls = 0;
    g_differs = differs ? 1 : 0;
    g_temporaryAddress = 0;
    g_destroyedTemporary = 0;
    g_createdComponent = &g_newComponent;
    g_createdDefinition = 0;
    g_createdFrontEnd = -1;
    g_createCalls = 0;
}

static bool KeysAreZero()
{
    for (int i = 0; i < g_resolveCalls; ++i)
        if (g_keys[i] != 0)
            return false;
    return true;
}

static bool RunMissing()
{
    unsigned char manager[0x80];
    unsigned long definition[2] = { 1, 2 };
    Reset(manager, false, false);
    CFrontEndManager_SetMainMenuDefName_00595a06(
        manager, 0, definition);
    static const Event expected[] =
    {
        kResolve, kResolve, kResolve, kGetManager, kCreate,
        kResolve, kActivate
    };
    return g_eventCount == 7 &&
        std::memcmp(g_events, expected, sizeof(expected)) == 0 &&
        g_resolveCalls == 4 &&
        KeysAreZero() &&
        g_mapValue == &g_newComponent &&
        g_createCalls == 1 &&
        g_createdDefinition == definition &&
        g_createdFrontEnd == 0 &&
        g_newComponent.activateCalls == 1;
}

static bool RunMatching()
{
    unsigned char manager[0x80];
    unsigned long definition[2] = { 3, 4 };
    Reset(manager, true, false);
    CFrontEndManager_SetMainMenuDefName_00595a06(
        manager, 0, definition);
    static const Event expected[] =
    {
        kResolve, kResolve, kGetDefinition, kCompare,
        kDestroyTemporary, kResolve
    };
    return g_eventCount == 6 &&
        std::memcmp(g_events, expected, sizeof(expected)) == 0 &&
        g_resolveCalls == 3 &&
        KeysAreZero() &&
        g_oldComponent.definitionCalls == 1 &&
        g_oldComponent.requestedDefinition == definition &&
        g_destroyedTemporary == g_temporaryAddress &&
        g_oldComponent.deleteCalls == 0 &&
        g_createCalls == 0 &&
        g_mapValue == &g_oldComponent;
}

static bool RunDifferent()
{
    unsigned char manager[0x80];
    unsigned long definition[2] = { 5, 6 };
    Reset(manager, true, true);
    CFrontEndManager_SetMainMenuDefName_00595a06(
        manager, 0, definition);
    static const Event expected[] =
    {
        kResolve, kResolve, kGetDefinition, kCompare,
        kDestroyTemporary, kResolve, kDelete, kResolve,
        kResolve, kResolve, kGetManager, kCreate, kResolve, kActivate
    };
    return g_eventCount == 14 &&
        std::memcmp(g_events, expected, sizeof(expected)) == 0 &&
        g_resolveCalls == 7 &&
        KeysAreZero() &&
        g_oldComponent.deleteCalls == 1 &&
        g_oldComponent.deleteFlag == 1 &&
        g_mapValue == &g_newComponent &&
        g_createCalls == 1 &&
        g_createdDefinition == definition &&
        g_createdFrontEnd == 0 &&
        g_newComponent.activateCalls == 1;
}

int main()
{
    const bool missing = RunMissing();
    const bool matching = RunMatching();
    const bool different = RunDifferent();
    if (!missing || !matching || !different)
    {
        std::printf(
            "FSE2_00595a06_TEST FAIL missing=%d matching=%d different=%d\n",
            missing ? 1 : 0,
            matching ? 1 : 0,
            different ? 1 : 0);
        return 1;
    }
    std::puts("FSE2_00595a06_TEST PASS");
    return 0;
}
