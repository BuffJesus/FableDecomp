// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::GotoNextScreen @ 0x00596763.

#include <cstdio>
#include <cstring>

struct Component
{
    void** vtable;
    unsigned long ownershipToken;
    unsigned char bytes[0x30];
    int lastState;
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

struct World
{
    void** vtable;
};

struct FrontEndManagerProbe
{
    unsigned char bytes[0xa0];
};

extern "C" void __fastcall
CFrontEndManager_GotoNextScreen_00596763(
    void* manager,
    void*,
    void* component,
    unsigned char transition);

extern "C" void* FableFrontEndGotoNextWorld;
extern "C" const float FableFrontEndGotoNextEffectScale = 1000.0f;

enum Event
{
    kTop = 1,
    kDeactivate,
    kResolve,
    kWorldEffect,
    kState,
    kOwnership,
    kSize,
    kPop,
    kPush,
    kGetDefinition,
    kPlaySound,
    kRelease
};

static Event g_events[64];
static int g_eventCount;
static void* g_stack[8];
static int g_stackCount;
static void* g_expectedStack;
static void* g_expectedManager;
static void* g_expectedUsedKeys;
static void* g_key26Component;
static unsigned long g_resolvedKey;
static int g_deactivateCalls;
static void* g_deactivateArguments[2];
static int g_sizeCalls;
static int g_popCalls;
static int g_pushCalls;
static void* g_pushedComponent;
static void* g_ownershipAddress;
static int g_getManagerCalls;
static int g_getDefinitionCalls;
static int g_playCalls;
static void* g_playCriteria;
static int g_playArgument;
static int g_releaseCalls;
static unsigned long g_effectFlags;
static float g_effectZero;
static float g_effectScale;

static void* g_componentVtable[0xc4 / 4];
static void* g_serviceVtable[0xac / 4];
static void* g_definitionVtable[2];
static void* g_worldVtable[0xbc / 4];
static Service g_service;
static Definition g_definition;
static World g_world;

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

extern "C" void** __fastcall
FableFrontEndGotoNextStackTop(void* stack, void*)
{
    AddEvent(kTop);
    if (stack != g_expectedStack || g_stackCount == 0)
        return 0;
    return &g_stack[g_stackCount - 1];
}

extern "C" void __fastcall
FableFrontEndGotoNextDeactivateEditBoxes(
    void* manager,
    void*,
    void* component)
{
    AddEvent(kDeactivate);
    if (manager != g_expectedManager)
        component = (void*)0xffffffff;
    g_deactivateArguments[g_deactivateCalls++] = component;
}

extern "C" void** __fastcall
FableFrontEndGotoNextResolveUsedKey(
    void* usedKeys,
    void*,
    const unsigned long* key)
{
    AddEvent(kResolve);
    g_resolvedKey =
        usedKeys == g_expectedUsedKeys ? *key : 0xffffffff;
    return &g_key26Component;
}

extern "C" long __fastcall
FableFrontEndGotoNextStackSize(
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

extern "C" void __fastcall
FableFrontEndGotoNextStackPop(void* stack, void*)
{
    AddEvent(kPop);
    ++g_popCalls;
    if (stack == g_expectedStack && g_stackCount != 0)
        --g_stackCount;
}

extern "C" void __fastcall
FableFrontEndGotoNextStackPush(
    void* stack,
    void*,
    void** component)
{
    AddEvent(kPush);
    ++g_pushCalls;
    g_pushedComponent = *component;
    if (stack == g_expectedStack)
        g_stack[g_stackCount++] = *component;
}

static void __fastcall
ComponentSetState(void* object, void*, int state)
{
    AddEvent(kState);
    Component* component = (Component*)object;
    component->lastState = state;
    ++component->stateCalls;
}

static void __fastcall
ServiceTakeOwnership(void*, void*, void* ownershipAddress)
{
    AddEvent(kOwnership);
    g_ownershipAddress = ownershipAddress;
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

static void __fastcall
DefinitionRelease(void*, void*)
{
    AddEvent(kRelease);
    ++g_releaseCalls;
}

static void __fastcall
WorldEffect(
    void*,
    void*,
    unsigned long flags,
    float zero,
    float scale)
{
    AddEvent(kWorldEffect);
    g_effectFlags = flags;
    g_effectZero = zero;
    g_effectScale = scale;
}

extern "C" void* __cdecl
FableFrontEndGotoNextGetManager()
{
    ++g_getManagerCalls;
    return &g_service;
}

static void Reset(FrontEndManagerProbe& manager)
{
    std::memset(&manager, 0, sizeof(manager));
    std::memset(g_stack, 0, sizeof(g_stack));
    std::memset(&g_definition, 0, sizeof(g_definition));
    g_eventCount = 0;
    g_stackCount = 0;
    g_expectedManager = &manager;
    g_expectedStack = (void*)((char*)&manager + 0x20);
    g_expectedUsedKeys = (void*)((char*)&manager + 0x54);
    g_key26Component = 0;
    g_resolvedKey = 0xffffffff;
    g_deactivateCalls = 0;
    g_deactivateArguments[0] = (void*)1;
    g_deactivateArguments[1] = (void*)1;
    g_sizeCalls = 0;
    g_popCalls = 0;
    g_pushCalls = 0;
    g_pushedComponent = 0;
    g_ownershipAddress = 0;
    g_getManagerCalls = 0;
    g_getDefinitionCalls = 0;
    g_playCalls = 0;
    g_playCriteria = 0;
    g_playArgument = 0;
    g_releaseCalls = 0;
    g_effectFlags = 0;
    g_effectZero = -1.0f;
    g_effectScale = -1.0f;

    g_definition.vtable = g_definitionVtable;
    g_definition.references = 1;
    FableFrontEndGotoNextWorld = &g_world;
}

static void InitialiseComponent(Component& component)
{
    std::memset(&component, 0, sizeof(component));
    component.vtable = g_componentVtable;
    component.ownershipToken = 0xabcdef01;
    component.lastState = -1;
}

static int CheckPushWithoutClearing()
{
    FrontEndManagerProbe manager;
    Reset(manager);
    Component root;
    Component old;
    Component target;
    InitialiseComponent(root);
    InitialiseComponent(old);
    InitialiseComponent(target);
    g_stack[0] = &root;
    g_stack[1] = &old;
    g_stackCount = 2;
    g_key26Component = &root;

    CFrontEndManager_GotoNextScreen_00596763(
        &manager,
        0,
        &target,
        0);

    if (g_resolvedKey != 0x1a ||
        g_deactivateCalls != 2 ||
        g_deactivateArguments[0] != 0 ||
        g_deactivateArguments[1] != 0)
        return 1;
    if (old.stateCalls != 1 || old.lastState != 6 ||
        g_ownershipAddress != (void*)((char*)&old + 4))
        return 2;
    if (g_sizeCalls != 0 || g_popCalls != 0 ||
        g_pushCalls != 1 || g_pushedComponent != &target ||
        g_stackCount != 3 ||
        g_stack[0] != &root ||
        g_stack[1] != &old ||
        g_stack[2] != &target)
        return 3;
    if (ManagerPointer(manager, 0x98) != &old ||
        ManagerPointer(manager, 0x9c) != &target)
        return 4;
    if (g_effectFlags != 0 ||
        g_getManagerCalls != 3 ||
        g_getDefinitionCalls != 1 ||
        g_playCalls != 1 ||
        g_playCriteria !=
            (void*)((char*)&g_definition + 0x1a0) ||
        g_playArgument != 1 ||
        g_definition.references != 0 ||
        g_releaseCalls != 1)
        return 5;
    return 0;
}

static int CheckClearAndUsedKeyEffect()
{
    FrontEndManagerProbe manager;
    Reset(manager);
    Component root;
    Component middle;
    Component old;
    Component target;
    InitialiseComponent(root);
    InitialiseComponent(middle);
    InitialiseComponent(old);
    InitialiseComponent(target);
    g_stack[0] = &root;
    g_stack[1] = &middle;
    g_stack[2] = &old;
    g_stackCount = 3;
    g_key26Component = &target;

    CFrontEndManager_GotoNextScreen_00596763(
        &manager,
        0,
        &target,
        1);

    if (g_effectFlags != 0x100 ||
        g_effectZero != 0.0f ||
        g_effectScale != 1000.0f)
        return 10;
    if (g_sizeCalls != 4 ||
        g_popCalls != 3 ||
        g_pushCalls != 1 ||
        g_stackCount != 1 ||
        g_stack[0] != &target)
        return 11;

    int firstSize = -1;
    int lastPop = -1;
    int push = -1;
    for (int i = 0; i != g_eventCount; ++i)
    {
        if (g_events[i] == kSize && firstSize == -1)
            firstSize = i;
        if (g_events[i] == kPop)
            lastPop = i;
        if (g_events[i] == kPush)
            push = i;
    }
    if (firstSize == -1 || lastPop < firstSize || push < lastPop)
        return 12;
    return 0;
}

static int CheckNullWorldFiltering()
{
    FrontEndManagerProbe manager;
    Reset(manager);
    Component old;
    Component target;
    InitialiseComponent(old);
    InitialiseComponent(target);
    g_stack[0] = &old;
    g_stackCount = 1;
    g_key26Component = &target;
    FableFrontEndGotoNextWorld = 0;
    g_definition.references = 2;

    CFrontEndManager_GotoNextScreen_00596763(
        &manager,
        0,
        &target,
        0);

    for (int i = 0; i != g_eventCount; ++i)
    {
        if (g_events[i] == kWorldEffect)
            return 20;
    }
    if (g_definition.references != 1 || g_releaseCalls != 0)
        return 21;
    return 0;
}

extern "C" void* FableFrontEndGotoNextWorld = 0;

int main()
{
    std::memset(g_componentVtable, 0, sizeof(g_componentVtable));
    std::memset(g_serviceVtable, 0, sizeof(g_serviceVtable));
    std::memset(g_definitionVtable, 0, sizeof(g_definitionVtable));
    std::memset(g_worldVtable, 0, sizeof(g_worldVtable));
    g_componentVtable[0xc0 / 4] = (void*)&ComponentSetState;
    g_serviceVtable[0x14 / 4] = (void*)&ServiceTakeOwnership;
    g_serviceVtable[0xa0 / 4] = (void*)&ServicePlaySound;
    g_serviceVtable[0xa8 / 4] = (void*)&ServiceGetDefinition;
    g_definitionVtable[1] = (void*)&DefinitionRelease;
    g_worldVtable[0xb8 / 4] = (void*)&WorldEffect;
    g_service.vtable = g_serviceVtable;
    g_world.vtable = g_worldVtable;

    int failure = CheckPushWithoutClearing();
    if (failure != 0)
        return failure;
    failure = CheckClearAndUsedKeyEffect();
    if (failure != 0)
        return failure;
    failure = CheckNullWorldFiltering();
    if (failure != 0)
        return failure;

    std::printf("FSE2_00596763_TEST PASS\n");
    return 0;
}
