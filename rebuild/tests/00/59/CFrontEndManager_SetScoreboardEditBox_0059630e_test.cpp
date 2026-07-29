// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::SetScoreboardEditBox @ 0x0059630E.

#include <cstdio>
#include <cstring>

extern "C" void __fastcall
CFrontEndManager_SetScoreboardEditBox_0059630e(
    void* manager,
    void*,
    const void* childName,
    const void* value);

struct NameProbe
{
    const char* text;
};

struct ComponentProbe
{
    void** vtable;
    long type;
    const void* value;
    int refreshCalls;
    long active;
};

enum Event
{
    kResolve = 1,
    kCopyName,
    kLookup,
    kType,
    kSetValue,
    kRefresh,
    kSetActive
};

static Event g_events[10];
static int g_eventCount;
static void* g_expectedUsedKeys;
static unsigned long g_key;
static void* g_screen;
static void* g_lookupResult;
static const void* g_copiedSource;
static const char* g_lookupName;
static void* g_screenVtable[4];
static void* g_controlVtable[0x25c / 4];
static ComponentProbe g_screenComponent;
static ComponentProbe g_control;

static void AddEvent(Event event)
{
    g_events[g_eventCount++] = event;
}

extern "C" void** __fastcall
FableFrontEndScoreboardSetResolveUsedKey(
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
FableFrontEndScoreboardSetCopyName(
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
ControlSetValue(void* object, void*, const void* value)
{
    AddEvent(kSetValue);
    ((ComponentProbe*)object)->value = value;
}

static void __fastcall
ControlRefresh(void* object, void*)
{
    AddEvent(kRefresh);
    ++((ComponentProbe*)object)->refreshCalls;
}

static void __fastcall
ControlSetActive(void* object, void*, long active)
{
    AddEvent(kSetActive);
    ((ComponentProbe*)object)->active = active;
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
    g_controlVtable[0x23c / 4] = (void*)ControlSetValue;
    g_controlVtable[0x24c / 4] = (void*)ControlRefresh;
    g_controlVtable[0x258 / 4] = (void*)ControlSetActive;
    g_screenComponent.vtable = g_screenVtable;
    g_control.vtable = g_controlVtable;
    g_control.type = type;
    g_control.active = 1;
    g_eventCount = 0;
    g_expectedUsedKeys = manager + 0x54;
    g_key = 0xffffffff;
    g_screen = &g_screenComponent;
    g_lookupResult = lookupResult;
    g_copiedSource = 0;
    g_lookupName = 0;
}

static bool Run(void* lookupResult, long type, int expectedEvents)
{
    unsigned char manager[0x80];
    NameProbe name = { "ScoreName" };
    unsigned long value[2] = { 1, 2 };
    Reset(manager, lookupResult, type);
    CFrontEndManager_SetScoreboardEditBox_0059630e(
        manager, 0, &name, value);

    static const Event nullEvents[] =
    {
        kResolve, kCopyName, kLookup
    };
    static const Event wrongEvents[] =
    {
        kResolve, kCopyName, kLookup, kType
    };
    static const Event editEvents[] =
    {
        kResolve, kCopyName, kLookup, kType,
        kSetValue, kRefresh, kSetActive
    };
    const Event* expected = expectedEvents == 3
        ? nullEvents
        : (expectedEvents == 4 ? wrongEvents : editEvents);
    const bool common =
        g_eventCount == expectedEvents &&
        std::memcmp(
            g_events,
            expected,
            expectedEvents * sizeof(Event)) == 0 &&
        g_key == 0x19 &&
        g_copiedSource == &name &&
        std::strcmp(g_lookupName, "ScoreName") == 0;
    return common &&
        (expectedEvents != 7 ||
            (g_control.value == value &&
             g_control.refreshCalls == 1 &&
             g_control.active == 0));
}

int main()
{
    const bool missing = Run(0, 0, 3);
    const bool wrongType = Run(&g_control, 0x24, 4);
    const bool edit = Run(&g_control, 0x25, 7);
    if (!missing || !wrongType || !edit)
    {
        std::printf(
            "FSE2_0059630e_TEST FAIL missing=%d wrong=%d edit=%d\n",
            missing ? 1 : 0,
            wrongType ? 1 : 0,
            edit ? 1 : 0);
        return 1;
    }
    std::puts("FSE2_0059630e_TEST PASS");
    return 0;
}
