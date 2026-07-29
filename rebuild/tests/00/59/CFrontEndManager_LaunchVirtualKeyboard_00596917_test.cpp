#include <cstdio>

#include "../../../src/compiled/00/59/CFrontEndManager_LaunchVirtualKeyboard_00596917.cpp"

enum KeyboardEvent_00596917
{
    EVENT_LOOKUP_00596917 = 1,
    EVENT_GOTO_00596917 = 2,
    EVENT_ALLOCATE_00596917 = 3,
    EVENT_CONSTRUCT_00596917 = 4,
    EVENT_INITIALIZE_00596917 = 5
};

static CFrontEndManager_00596917 g_Manager;
static CComponent_00596917 g_RouteComponent;
static CComponent_00596917* g_Route;
static CVirtualKeyboard_00596917 g_KeyboardStorage;
static bool g_AllocationSucceeds;
static int g_Events[8];
static int g_EventCount;
static int g_LookupCalls;
static int g_LookupKeys[2];
static int g_GotoCalls;
static CFrontEndManager_00596917* g_GotoManager;
static CComponent_00596917* g_GotoComponent;
static bool g_GotoImmediate;
static int g_AllocateCalls;
static unsigned int g_AllocatedBytes;
static int g_ConstructCalls;
static CVirtualKeyboard_00596917* g_ConstructThis;
static CComponent_00596917* g_ConstructComponent;
static int g_InitializeCalls;
static CVirtualKeyboard_00596917* g_InitializeThis;
static bool g_StoredBeforeInitialize;

static void RecordEvent(int event)
{
    g_Events[g_EventCount++] = event;
}

extern "C" CComponent_00596917** __fastcall
FrontEndUsedComponentLookup_00596917(
    FrontEndUsedComponentMap_00596917* usedComponents,
    void*,
    const int* key)
{
    RecordEvent(EVENT_LOOKUP_00596917);
    if (usedComponents == &g_Manager.usedComponents &&
        g_LookupCalls < 2)
    {
        g_LookupKeys[g_LookupCalls] = *key;
    }
    ++g_LookupCalls;
    return &g_Route;
}

extern "C" void __fastcall FrontEndGotoNextScreen_00596917(
    CFrontEndManager_00596917* manager,
    void*,
    CComponent_00596917* component,
    bool immediate)
{
    RecordEvent(EVENT_GOTO_00596917);
    ++g_GotoCalls;
    g_GotoManager = manager;
    g_GotoComponent = component;
    g_GotoImmediate = immediate;
}

extern "C" void* __cdecl FrontEndKeyboardAllocate_00596917(
    unsigned int bytes)
{
    RecordEvent(EVENT_ALLOCATE_00596917);
    ++g_AllocateCalls;
    g_AllocatedBytes = bytes;
    return g_AllocationSucceeds ? &g_KeyboardStorage : 0;
}

extern "C" CVirtualKeyboard_00596917* __fastcall
FrontEndKeyboardConstruct_00596917(
    CVirtualKeyboard_00596917* keyboard,
    void*,
    CComponent_00596917* component)
{
    RecordEvent(EVENT_CONSTRUCT_00596917);
    ++g_ConstructCalls;
    g_ConstructThis = keyboard;
    g_ConstructComponent = component;
    return keyboard;
}

extern "C" long __fastcall FrontEndKeyboardInitialize_00596917(
    CVirtualKeyboard_00596917* keyboard,
    void*)
{
    RecordEvent(EVENT_INITIALIZE_00596917);
    ++g_InitializeCalls;
    g_InitializeThis = keyboard;
    g_StoredBeforeInitialize = g_Manager.virtualKeyboard == keyboard;
    return 1;
}

static void ResetFixture(bool allocationSucceeds)
{
    g_Route = &g_RouteComponent;
    g_Manager.virtualKeyboard =
        (CVirtualKeyboard_00596917*)0x11111111;
    g_AllocationSucceeds = allocationSucceeds;
    g_EventCount = 0;
    g_LookupCalls = 0;
    g_LookupKeys[0] = 0;
    g_LookupKeys[1] = 0;
    g_GotoCalls = 0;
    g_GotoManager = 0;
    g_GotoComponent = 0;
    g_GotoImmediate = true;
    g_AllocateCalls = 0;
    g_AllocatedBytes = 0;
    g_ConstructCalls = 0;
    g_ConstructThis = 0;
    g_ConstructComponent = 0;
    g_InitializeCalls = 0;
    g_InitializeThis = (CVirtualKeyboard_00596917*)0x22222222;
    g_StoredBeforeInitialize = false;
}

static int CheckRoutingAndAllocation()
{
    int failures = 0;
    if (g_GotoCalls != 1 ||
        g_GotoManager != &g_Manager ||
        g_GotoComponent != g_Route ||
        g_GotoImmediate)
    {
        std::printf("screen routing contract failed\n");
        ++failures;
    }
    if (g_AllocateCalls != 1 || g_AllocatedBytes != 0x10)
    {
        std::printf("keyboard allocation contract failed\n");
        ++failures;
    }
    return failures;
}

int main()
{
    int failures = 0;

    if ((char*)&g_Manager.usedComponents - (char*)&g_Manager != 0x54 ||
        (char*)&g_Manager.virtualKeyboard - (char*)&g_Manager != 0x60 ||
        sizeof(CVirtualKeyboard_00596917) != 0x10)
    {
        std::printf("recovered layout contract failed\n");
        ++failures;
    }

    // Successful allocation performs a second route lookup for construction,
    // stores the constructor result, then initializes that stored object.
    ResetFixture(true);
    g_Manager.LaunchVirtualKeyboard();
    failures += CheckRoutingAndAllocation();
    const int expectedSuccessEvents[6] =
    {
        EVENT_LOOKUP_00596917,
        EVENT_GOTO_00596917,
        EVENT_ALLOCATE_00596917,
        EVENT_LOOKUP_00596917,
        EVENT_CONSTRUCT_00596917,
        EVENT_INITIALIZE_00596917
    };
    if (g_LookupCalls != 2 ||
        g_LookupKeys[0] != 0x17 ||
        g_LookupKeys[1] != 0x17 ||
        g_ConstructCalls != 1 ||
        g_ConstructThis != &g_KeyboardStorage ||
        g_ConstructComponent != g_Route ||
        g_Manager.virtualKeyboard != &g_KeyboardStorage ||
        g_InitializeCalls != 1 ||
        g_InitializeThis != &g_KeyboardStorage ||
        !g_StoredBeforeInitialize ||
        g_EventCount != 6)
    {
        std::printf("successful keyboard construction failed\n");
        ++failures;
    }
    for (int index = 0; index < 6; ++index)
    {
        if (g_Events[index] != expectedSuccessEvents[index])
        {
            std::printf("successful call order failed\n");
            ++failures;
            break;
        }
    }

    // Retail still stores null and invokes Initialize with a null this pointer
    // when the non-throwing allocation path returns null.
    ResetFixture(false);
    g_Manager.LaunchVirtualKeyboard();
    failures += CheckRoutingAndAllocation();
    const int expectedFailureEvents[4] =
    {
        EVENT_LOOKUP_00596917,
        EVENT_GOTO_00596917,
        EVENT_ALLOCATE_00596917,
        EVENT_INITIALIZE_00596917
    };
    if (g_LookupCalls != 1 ||
        g_LookupKeys[0] != 0x17 ||
        g_ConstructCalls != 0 ||
        g_Manager.virtualKeyboard != 0 ||
        g_InitializeCalls != 1 ||
        g_InitializeThis != 0 ||
        !g_StoredBeforeInitialize ||
        g_EventCount != 4)
    {
        std::printf("null-allocation path failed\n");
        ++failures;
    }
    for (int index = 0; index < 4; ++index)
    {
        if (g_Events[index] != expectedFailureEvents[index])
        {
            std::printf("null-allocation call order failed\n");
            ++failures;
            break;
        }
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_00596917_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_00596917_TEST PASS\n");
    return 0;
}
