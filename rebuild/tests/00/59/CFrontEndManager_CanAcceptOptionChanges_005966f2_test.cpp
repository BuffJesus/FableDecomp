// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::CanAcceptOptionChanges @ 0x005966F2.

#include <cstdio>
#include <cstring>

extern "C" unsigned char __fastcall
CFrontEndManager_CanAcceptOptionChanges_005966f2(void*, void*);

enum Event
{
    kResolve = 1,
    kTop,
    kValidate
};

static Event g_events[4];
static int g_eventCount;
static void* g_expectedUsedKeys;
static void* g_expectedStack;
static unsigned long g_resolvedKey;
static long g_optionsComponent;
static long g_otherComponent;
static long* g_resolvedPointer;
static long* g_topPointer;
static unsigned char g_validationResult;
static int g_validationCalls;

static void AddEvent(Event event)
{
    g_events[g_eventCount++] = event;
}

extern "C" long** __fastcall
FableFrontEndCanAcceptResolveUsedKey(
    void* usedKeys,
    void*,
    const unsigned long* key)
{
    AddEvent(kResolve);
    if (usedKeys == g_expectedUsedKeys)
        g_resolvedKey = *key;
    return &g_resolvedPointer;
}

extern "C" long** __fastcall
FableFrontEndCanAcceptStackTop(void* stack, void*)
{
    AddEvent(kTop);
    if (stack != g_expectedStack)
        g_topPointer = 0;
    return &g_topPointer;
}

extern "C" unsigned char __cdecl
FableFrontEndCanAcceptChanges()
{
    AddEvent(kValidate);
    ++g_validationCalls;
    return g_validationResult;
}

static void Reset(unsigned char* manager)
{
    std::memset(manager, 0, 0x80);
    g_eventCount = 0;
    g_expectedUsedKeys = manager + 0x54;
    g_expectedStack = manager + 0x20;
    g_resolvedKey = 0xffffffff;
    g_optionsComponent = 0x11223344;
    g_otherComponent = 0x55667788;
    g_resolvedPointer = &g_optionsComponent;
    g_topPointer = &g_optionsComponent;
    g_validationResult = 0;
    g_validationCalls = 0;
}

static bool RunCurrent(unsigned char validatorResult)
{
    unsigned char manager[0x80];
    Reset(manager);
    g_validationResult = validatorResult;
    const unsigned char result =
        CFrontEndManager_CanAcceptOptionChanges_005966f2(manager, 0);
    static const Event expected[] = { kResolve, kTop, kValidate };
    return result == validatorResult &&
        g_eventCount == 3 &&
        std::memcmp(g_events, expected, sizeof(expected)) == 0 &&
        g_resolvedKey == 0x16 &&
        g_validationCalls == 1;
}

static bool RunOther()
{
    unsigned char manager[0x80];
    Reset(manager);
    g_topPointer = &g_otherComponent;
    const unsigned char result =
        CFrontEndManager_CanAcceptOptionChanges_005966f2(manager, 0);
    static const Event expected[] = { kResolve, kTop };
    return result == 1 &&
        g_eventCount == 2 &&
        std::memcmp(g_events, expected, sizeof(expected)) == 0 &&
        g_resolvedKey == 0x16 &&
        g_validationCalls == 0;
}

int main()
{
    const bool rejected = RunCurrent(0);
    const bool accepted = RunCurrent(1);
    const bool other = RunOther();
    if (!rejected || !accepted || !other)
    {
        std::printf(
            "FSE2_005966f2_TEST FAIL rejected=%d accepted=%d other=%d\n",
            rejected ? 1 : 0,
            accepted ? 1 : 0,
            other ? 1 : 0);
        return 1;
    }
    std::puts("FSE2_005966f2_TEST PASS");
    return 0;
}
