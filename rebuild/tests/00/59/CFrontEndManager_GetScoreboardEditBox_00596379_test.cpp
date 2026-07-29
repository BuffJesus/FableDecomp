// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::GetScoreboardEditBox @ 0x00596379.

#include <cstdio>
#include <cstring>

extern "C" void* __fastcall
CFrontEndManager_GetScoreboardEditBox_00596379(
    void* manager,
    void*,
    void* output,
    const void* childName);

struct NameProbe
{
    const char* text;
};

struct WideProbe
{
    unsigned long value;
};

struct ComponentProbe
{
    void** vtable;
    long type;
};

enum Event
{
    kResolve = 1,
    kCopyName,
    kLookup,
    kType,
    kGetValue,
    kDefault
};

static Event g_events[8];
static int g_eventCount;
static void* g_expectedUsedKeys;
static unsigned long g_key;
static void* g_screen;
static void* g_lookupResult;
static const void* g_copiedSource;
static const char* g_lookupName;
static void* g_screenVtable[4];
static void* g_controlVtable[0x23c / 4];
static ComponentProbe g_screenComponent;
static ComponentProbe g_control;

static void AddEvent(Event event)
{
    g_events[g_eventCount++] = event;
}

extern "C" void** __fastcall
FableFrontEndScoreboardGetResolveUsedKey(
    void* usedKeys,
    void*,
    const unsigned long* key)
{
    AddEvent(kResolve);
    if (usedKeys == g_expectedUsedKeys)
        g_key = *key;
    return &g_screen;
}

extern "C" void __fastcall
FableFrontEndScoreboardGetCopyName(
    void* destination,
    void*,
    const void* source)
{
    AddEvent(kCopyName);
    g_copiedSource = source;
    *(NameProbe*)destination = *(const NameProbe*)source;
}

static void* __fastcall
ScreenLookup(void* object, void*, NameProbe name)
{
    AddEvent(kLookup);
    if (object == &g_screenComponent)
        g_lookupName = name.text;
    return g_lookupResult;
}

static long __fastcall
ControlType(void* object, void*)
{
    AddEvent(kType);
    return ((ComponentProbe*)object)->type;
}

static void __fastcall
ControlGetValue(void* object, void*, void* output)
{
    AddEvent(kGetValue);
    if (object == &g_control)
        ((WideProbe*)output)->value = 0x12345678;
}

extern "C" void __fastcall
FableFrontEndScoreboardGetConstructDefault(void* output, void*)
{
    AddEvent(kDefault);
    ((WideProbe*)output)->value = 0;
}

static void Reset(unsigned char* manager, void* lookupResult, long type)
{
    std::memset(manager, 0, 0x80);
    std::memset(g_screenVtable, 0, sizeof(g_screenVtable));
    std::memset(g_controlVtable, 0, sizeof(g_controlVtable));
    std::memset(&g_screenComponent, 0, sizeof(g_screenComponent));
    std::memset(&g_control, 0, sizeof(g_control));
    g_screenVtable[0x0c / 4] = (void*)ScreenLookup;
    g_controlVtable[0x104 / 4] = (void*)ControlType;
    g_controlVtable[0x238 / 4] = (void*)ControlGetValue;
    g_screenComponent.vtable = g_screenVtable;
    g_control.vtable = g_controlVtable;
    g_control.type = type;
    g_eventCount = 0;
    g_expectedUsedKeys = manager + 0x54;
    g_key = 0xffffffff;
    g_screen = &g_screenComponent;
    g_lookupResult = lookupResult;
    g_copiedSource = 0;
    g_lookupName = 0;
}

static bool Run(void* lookupResult, long type, bool valid)
{
    unsigned char manager[0x80];
    NameProbe name = { "ScoreName" };
    WideProbe output = { 0xffffffff };
    Reset(manager, lookupResult, type);
    void* result = CFrontEndManager_GetScoreboardEditBox_00596379(
        manager, 0, &output, &name);

    static const Event nullEvents[] =
    {
        kResolve, kCopyName, kLookup, kDefault
    };
    static const Event wrongEvents[] =
    {
        kResolve, kCopyName, kLookup, kType, kDefault
    };
    static const Event validEvents[] =
    {
        kResolve, kCopyName, kLookup, kType, kGetValue
    };
    const Event* expected = valid
        ? validEvents
        : (lookupResult ? wrongEvents : nullEvents);
    const int count = lookupResult ? 5 : 4;
    return result == &output &&
        g_eventCount == count &&
        std::memcmp(g_events, expected, count * sizeof(Event)) == 0 &&
        g_key == 0x19 &&
        g_copiedSource == &name &&
        std::strcmp(g_lookupName, "ScoreName") == 0 &&
        output.value == (valid ? 0x12345678 : 0);
}

int main()
{
    const bool missing = Run(0, 0, false);
    const bool wrongType = Run(&g_control, 0x24, false);
    const bool valid = Run(&g_control, 0x25, true);
    if (!missing || !wrongType || !valid)
    {
        std::printf(
            "FSE2_00596379_TEST FAIL missing=%d wrong=%d valid=%d\n",
            missing ? 1 : 0,
            wrongType ? 1 : 0,
            valid ? 1 : 0);
        return 1;
    }
    std::puts("FSE2_00596379_TEST PASS");
    return 0;
}
