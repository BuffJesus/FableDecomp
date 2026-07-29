// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::AddPrefixToTitleBar @ 0x00595AD9.

#include <cstdio>
#include <cstring>

extern "C" void __fastcall
CFrontEndManager_AddPrefixToTitleBar_00595ad9(
    void* manager,
    void*,
    unsigned long screen,
    const void* childName,
    const void* prefix);

struct StringProbe
{
    const char* text;
};

struct ComponentProbe
{
    void** vtable;
};

enum Event
{
    kResolve = 1,
    kCopyName,
    kVirtualLookup,
    kResetText
};

static Event g_events[8];
static int g_eventCount;
static void* g_expectedUsedKeys;
static unsigned long g_resolvedScreens[2];
static int g_resolveCalls;
static void* g_resolvedComponent;
static const void* g_copiedSource;
static const char* g_virtualName;
static void* g_virtualResult;
static void* g_resetText;
static const void* g_resetPrefix;
static void* g_componentVtable[4];
static ComponentProbe g_component;
static unsigned char g_text;

static void AddEvent(Event event)
{
    g_events[g_eventCount++] = event;
}

extern "C" void** __fastcall
FableFrontEndTitlePrefixResolveUsedKey(
    void* usedKeys,
    void*,
    const unsigned long* screen)
{
    AddEvent(kResolve);
    if (usedKeys == g_expectedUsedKeys)
        g_resolvedScreens[g_resolveCalls] = *screen;
    ++g_resolveCalls;
    return &g_resolvedComponent;
}

extern "C" void __fastcall
FableFrontEndTitlePrefixCopyName(
    void* destination,
    void*,
    const void* source)
{
    AddEvent(kCopyName);
    g_copiedSource = source;
    *(StringProbe*)destination = *(const StringProbe*)source;
}

static void* __fastcall
ComponentLookup(void* component, void*, StringProbe name)
{
    AddEvent(kVirtualLookup);
    if (component == &g_component)
        g_virtualName = name.text;
    return g_virtualResult;
}

extern "C" void __fastcall
FableFrontEndTitlePrefixResetText(
    void* text,
    void*,
    const void* prefix)
{
    AddEvent(kResetText);
    g_resetText = text;
    g_resetPrefix = prefix;
}

static void Reset(unsigned char* manager)
{
    std::memset(manager, 0, 0x80);
    g_eventCount = 0;
    g_expectedUsedKeys = manager + 0x54;
    g_resolvedScreens[0] = 0xffffffff;
    g_resolvedScreens[1] = 0xffffffff;
    g_resolveCalls = 0;
    g_resolvedComponent = 0;
    g_copiedSource = 0;
    g_virtualName = 0;
    g_virtualResult = 0;
    g_resetText = 0;
    g_resetPrefix = 0;
    std::memset(g_componentVtable, 0, sizeof(g_componentVtable));
    g_componentVtable[3] = (void*)ComponentLookup;
    g_component.vtable = g_componentVtable;
}

static bool RunMissing()
{
    unsigned char manager[0x80];
    StringProbe name = { "Title" };
    unsigned long prefix[2] = { 1, 2 };
    Reset(manager);

    CFrontEndManager_AddPrefixToTitleBar_00595ad9(
        manager, 0, 7, &name, prefix);

    return g_eventCount == 1 &&
        g_events[0] == kResolve &&
        g_resolveCalls == 1 &&
        g_resolvedScreens[0] == 7 &&
        g_copiedSource == 0 &&
        g_resetText == 0;
}

static bool RunFound(bool returnText)
{
    unsigned char manager[0x80];
    StringProbe name = { "Title" };
    unsigned long prefix[2] = { 3, 4 };
    Reset(manager);
    g_resolvedComponent = &g_component;
    g_virtualResult = returnText ? &g_text : 0;

    CFrontEndManager_AddPrefixToTitleBar_00595ad9(
        manager, 0, 0x0b, &name, prefix);

    static const Event withoutText[] =
    {
        kResolve, kResolve, kCopyName, kVirtualLookup
    };
    static const Event withText[] =
    {
        kResolve, kResolve, kCopyName, kVirtualLookup, kResetText
    };
    const Event* expected = returnText ? withText : withoutText;
    const unsigned long count = returnText
        ? sizeof(withText) / sizeof(withText[0])
        : sizeof(withoutText) / sizeof(withoutText[0]);
    return g_eventCount == (int)count &&
        std::memcmp(g_events, expected, count * sizeof(Event)) == 0 &&
        g_resolveCalls == 2 &&
        g_resolvedScreens[0] == 0x0b &&
        g_resolvedScreens[1] == 0x0b &&
        g_copiedSource == &name &&
        std::strcmp(g_virtualName, "Title") == 0 &&
        (returnText
            ? g_resetText == &g_text && g_resetPrefix == prefix
            : g_resetText == 0 && g_resetPrefix == 0);
}

int main()
{
    const bool missing = RunMissing();
    const bool noText = RunFound(false);
    const bool found = RunFound(true);
    if (!missing || !noText || !found)
    {
        std::printf(
            "FSE2_00595ad9_TEST FAIL missing=%d noText=%d found=%d\n",
            missing ? 1 : 0,
            noText ? 1 : 0,
            found ? 1 : 0);
        return 1;
    }
    std::puts("FSE2_00595ad9_TEST PASS");
    return 0;
}
