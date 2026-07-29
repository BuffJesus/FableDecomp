// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::GotoPreviousScreen @ 0x00597BF2.

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

struct Keyboard
{
    unsigned char bytes[0x10];
};

struct FrontEndManagerProbe
{
    unsigned char bytes[0xa0];
};

extern "C" void __fastcall
CFrontEndManager_GotoPreviousScreen_00597bf2(
    void* manager,
    void*,
    unsigned char transition);

extern "C" void* FableFrontEndGotoPreviousGame = 0;

enum Event
{
    kTop = 1,
    kResolve,
    kLoad,
    kSynchronize,
    kFinish,
    kSize,
    kIterate,
    kSpecific,
    kState,
    kOwnership,
    kPop,
    kNew,
    kKeyboardConstruct,
    kKeyboardInitialize,
    kNext,
    kGetDefinition,
    kPlaySound,
    kRelease
};

static Event g_events[80];
static int g_eventCount;
static void* g_stack[8];
static int g_stackCount;
static void* g_expectedManager;
static void* g_expectedStack;
static void* g_expectedUsedKeys;
static long* g_usedKeys[0x20];
static unsigned long g_resolvedKeys[16];
static int g_resolveCount;
static int g_topCalls;
static int g_sizeCalls;
static int g_popCalls;
static unsigned char g_iterateResult;
static void* g_iteratedComponent;
static void* g_specificComponent;
static void* g_ownershipAddress;
static void* g_nextComponent;
static unsigned char g_nextTransition;
static int g_nextCalls;
static unsigned long g_newBytes;
static void* g_newResult;
static void* g_keyboardComponent;
static void* g_initializedKeyboard;
static int g_getProfileCalls;
static int g_getManagerCalls;
static int g_loadCalls;
static int g_synchronizeCalls;
static int g_finishCalls;
static int g_playCalls;
static void* g_playCriteria;
static int g_playArgument;
static int g_releaseCalls;

static void* g_componentVtable[0xc4 / 4];
static void* g_serviceVtable[0xac / 4];
static void* g_definitionVtable[2];
static Service g_service;
static Definition g_definition;
static ProfileManager g_profile;
static Keyboard g_keyboard;
static int g_game;

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
FableFrontEndGotoPreviousStackTop(void* stack, void*)
{
    AddEvent(kTop);
    ++g_topCalls;
    if (stack != g_expectedStack || g_stackCount == 0)
        return 0;
    return &g_stack[g_stackCount - 1];
}

extern "C" long** __fastcall
FableFrontEndGotoPreviousResolveUsedKey(
    void* usedKeys,
    void*,
    const unsigned long* key)
{
    AddEvent(kResolve);
    const unsigned long resolved =
        usedKeys == g_expectedUsedKeys ? *key : 0xffffffff;
    g_resolvedKeys[g_resolveCount++] = resolved;
    return &g_usedKeys[resolved & 0x1f];
}

extern "C" void* __cdecl
FableFrontEndGotoPreviousGetProfileManager()
{
    ++g_getProfileCalls;
    return &g_profile;
}

extern "C" void __fastcall
FableFrontEndGotoPreviousLoadProfileManager(void* profileManager, void*)
{
    AddEvent(kLoad);
    if (profileManager == &g_profile)
        ++g_loadCalls;
}

extern "C" void __fastcall
FableFrontEndGotoPreviousSynchronizeGame(void* game, void*)
{
    AddEvent(kSynchronize);
    if (game == &g_game)
        ++g_synchronizeCalls;
}

extern "C" void __fastcall
FableFrontEndGotoPreviousFinishDynamicTextures(
    void* manager,
    void*,
    int immediate)
{
    AddEvent(kFinish);
    if (manager == g_expectedManager && immediate == 0)
        ++g_finishCalls;
}

extern "C" unsigned long __fastcall
FableFrontEndGotoPreviousStackSize(
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

extern "C" unsigned char __fastcall
FableFrontEndGotoPreviousIterateElements(
    void* manager,
    void*,
    void* component)
{
    AddEvent(kIterate);
    if (manager == g_expectedManager)
        g_iteratedComponent = component;
    return g_iterateResult;
}

extern "C" void __fastcall
FableFrontEndGotoPreviousSpecific(
    void* manager,
    void*,
    void* component)
{
    AddEvent(kSpecific);
    if (manager == g_expectedManager)
        g_specificComponent = component;
}

extern "C" void __fastcall
FableFrontEndGotoPreviousStackPop(void* stack, void*)
{
    AddEvent(kPop);
    ++g_popCalls;
    if (stack == g_expectedStack && g_stackCount != 0)
        --g_stackCount;
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

static Definition** __fastcall
ServiceGetDefinition(void*, void*, Definition** result)
{
    AddEvent(kGetDefinition);
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

extern "C" void* __cdecl
FableFrontEndGotoPreviousGetManager()
{
    ++g_getManagerCalls;
    return &g_service;
}

extern "C" void* __cdecl
FableFrontEndGotoPreviousNew(unsigned long bytes)
{
    AddEvent(kNew);
    g_newBytes = bytes;
    return g_newResult;
}

extern "C" void* __fastcall
FableFrontEndGotoPreviousKeyboardConstruct(
    void* storage,
    void*,
    void* component)
{
    AddEvent(kKeyboardConstruct);
    g_keyboardComponent = component;
    return storage;
}

extern "C" void __fastcall
FableFrontEndGotoPreviousKeyboardInitialize(void* keyboard, void*)
{
    AddEvent(kKeyboardInitialize);
    g_initializedKeyboard = keyboard;
}

extern "C" void __fastcall
FableFrontEndGotoPreviousNext(
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
    std::memset(&g_profile, 0, sizeof(g_profile));
    std::memset(&g_definition, 0, sizeof(g_definition));
    g_definition.vtable = g_definitionVtable;
    g_definition.references = 1;
    g_eventCount = 0;
    g_stackCount = 0;
    g_expectedManager = &manager;
    g_expectedStack = (void*)((char*)&manager + 0x20);
    g_expectedUsedKeys = (void*)((char*)&manager + 0x54);
    g_resolveCount = 0;
    g_topCalls = 0;
    g_sizeCalls = 0;
    g_popCalls = 0;
    g_iterateResult = 0;
    g_iteratedComponent = 0;
    g_specificComponent = 0;
    g_ownershipAddress = 0;
    g_nextComponent = 0;
    g_nextTransition = 0xff;
    g_nextCalls = 0;
    g_newBytes = 0;
    g_newResult = &g_keyboard;
    g_keyboardComponent = 0;
    g_initializedKeyboard = 0;
    g_getProfileCalls = 0;
    g_getManagerCalls = 0;
    g_loadCalls = 0;
    g_synchronizeCalls = 0;
    g_finishCalls = 0;
    g_playCalls = 0;
    g_playCriteria = 0;
    g_playArgument = 0;
    g_releaseCalls = 0;
    FableFrontEndGotoPreviousGame = &g_game;
}

static void InitialiseComponent(Component& component)
{
    std::memset(&component, 0, sizeof(component));
    component.vtable = g_componentVtable;
    component.ownershipToken = 0x1234abcd;
    component.state = -1;
}

static int CheckNormalPopAndPublish(bool keyboardScreen)
{
    FrontEndManagerProbe manager;
    Reset(manager);
    Component previous;
    Component current;
    Component other;
    InitialiseComponent(previous);
    InitialiseComponent(current);
    InitialiseComponent(other);
    g_stack[0] = &previous;
    g_stack[1] = &current;
    g_stackCount = 2;
    g_usedKeys[0x14] = (long*)&other;
    g_usedKeys[0x1b] = (long*)&other;
    g_usedKeys[0x17] =
        (long*)(keyboardScreen ? &previous : &other);

    CFrontEndManager_GotoPreviousScreen_00597bf2(
        &manager,
        0,
        0);

    if (g_loadCalls != 1 ||
        g_synchronizeCalls != 1 ||
        g_finishCalls != 1 ||
        g_sizeCalls != 1)
        return 1;
    if (current.stateCalls != 1 ||
        current.state != 6 ||
        g_ownershipAddress != (void*)((char*)&current + 4) ||
        g_popCalls != 1 ||
        g_stackCount != 1 ||
        g_stack[0] != &previous)
        return 2;
    if (ManagerPointer(manager, 0x98) != &current ||
        ManagerPointer(manager, 0x9c) != &previous)
        return 3;
    if (g_playCalls != 1 ||
        g_playCriteria !=
            (void*)((char*)&g_definition + 0x19c) ||
        g_playArgument != 1 ||
        g_definition.references != 0 ||
        g_releaseCalls != 1)
        return 4;

    if (keyboardScreen)
    {
        if (g_profile.bytes[0x0c] != 1 ||
            g_newBytes != 0x10 ||
            g_keyboardComponent != &previous ||
            ManagerPointer(manager, 0x60) != &g_keyboard ||
            g_initializedKeyboard != &g_keyboard)
            return 5;
    }
    else if (g_newBytes != 0 ||
             ManagerPointer(manager, 0x60) != 0)
    {
        return 6;
    }
    return 0;
}

static int CheckProtectedRootTransition()
{
    FrontEndManagerProbe manager;
    Reset(manager);
    Component root;
    Component fallback;
    InitialiseComponent(root);
    InitialiseComponent(fallback);
    g_stack[0] = &root;
    g_stackCount = 1;
    g_usedKeys[0x14] = (long*)&root;
    g_usedKeys[0x1a] = (long*)&fallback;

    CFrontEndManager_GotoPreviousScreen_00597bf2(
        &manager,
        0,
        0);
    if (g_nextCalls != 0 ||
        g_playCalls != 0 ||
        g_getProfileCalls != 0)
        return 10;

    Reset(manager);
    g_stack[0] = &root;
    g_stackCount = 1;
    g_usedKeys[0x14] = (long*)&root;
    g_usedKeys[0x1a] = (long*)&fallback;
    CFrontEndManager_GotoPreviousScreen_00597bf2(
        &manager,
        0,
        1);
    if (g_nextCalls != 1 ||
        g_nextComponent != &fallback ||
        g_nextTransition != 0 ||
        g_playCalls != 1)
        return 11;
    return 0;
}

static int CheckSpecialUsedKeyBranches()
{
    FrontEndManagerProbe manager;
    Reset(manager);
    Component previous;
    Component special;
    Component key7;
    Component key20;
    InitialiseComponent(previous);
    InitialiseComponent(special);
    InitialiseComponent(key7);
    InitialiseComponent(key20);
    g_stack[0] = &previous;
    g_stack[1] = &special;
    g_stackCount = 2;
    g_usedKeys[0x14] = (long*)&previous;
    g_usedKeys[0x1b] = (long*)&special;
    g_usedKeys[0x07] = (long*)&key7;
    g_usedKeys[0x14] = (long*)&key20;
    g_iterateResult = 1;

    CFrontEndManager_GotoPreviousScreen_00597bf2(
        &manager,
        0,
        0);
    if (g_iteratedComponent != &key7 ||
        g_specificComponent != &key7 ||
        g_popCalls != 0 ||
        g_nextCalls != 0)
        return 20;

    Reset(manager);
    g_stack[0] = &previous;
    g_stack[1] = &special;
    g_stackCount = 2;
    g_usedKeys[0x14] = (long*)&key20;
    g_usedKeys[0x1b] = (long*)&special;
    g_usedKeys[0x07] = (long*)&key7;
    g_iterateResult = 0;
    CFrontEndManager_GotoPreviousScreen_00597bf2(
        &manager,
        0,
        0);
    if (g_nextCalls != 1 ||
        g_nextComponent != &key20 ||
        g_nextTransition != 1 ||
        g_popCalls != 0)
        return 21;
    return 0;
}

static int CheckRetainedSoundReference()
{
    FrontEndManagerProbe manager;
    Reset(manager);
    Component only;
    Component key20;
    InitialiseComponent(only);
    InitialiseComponent(key20);
    g_stack[0] = &only;
    g_stackCount = 1;
    g_usedKeys[0x14] = (long*)&key20;
    g_definition.references = 2;

    CFrontEndManager_GotoPreviousScreen_00597bf2(
        &manager,
        0,
        0);
    if (g_popCalls != 0 ||
        g_playCalls != 1 ||
        g_definition.references != 1 ||
        g_releaseCalls != 0)
        return 30;
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

    int failure = CheckNormalPopAndPublish(false);
    if (failure != 0)
        return failure;
    failure = CheckNormalPopAndPublish(true);
    if (failure != 0)
        return 40 + failure;
    failure = CheckProtectedRootTransition();
    if (failure != 0)
        return failure;
    failure = CheckSpecialUsedKeyBranches();
    if (failure != 0)
        return failure;
    failure = CheckRetainedSoundReference();
    if (failure != 0)
        return failure;

    std::printf("FSE2_00597bf2_TEST PASS\n");
    return 0;
}
