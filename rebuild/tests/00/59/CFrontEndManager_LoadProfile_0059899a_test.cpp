// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::LoadProfile @ 0x0059899A.

#include <cstdio>
#include <cstring>

extern "C" void __fastcall
CFrontEndManager_LoadProfile_0059899a(void*, void*, void*);

extern "C" const char
FableFrontEndLoadProfileNoLiveAwareNoContinue[] =
    "UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE_NO_CONTINUE";
extern "C" const char
FableFrontEndLoadProfileNoLiveAware[] =
    "UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE";

enum Event
{
    kGetService = 1,
    kLoadSelected,
    kGetSavedGameNames,
    kConstructString,
    kSetMainMenuDefName,
    kDestroyString,
    kAddProfileName,
    kDestroyNames
};

struct ServiceProbe
{
    unsigned char bytes[0x10];
};

struct NamesProbe
{
    void* first;
    void* last;
    void* end;
};

struct StringProbe
{
    const char* text;
};

static Event g_events[16];
static int g_eventCount;
static ServiceProbe g_service;
static void* g_expectedManager;
static void* g_expectedProfileName;
static int g_serviceCalls;
static void* g_loadedProfileName;
static long g_activate;
static bool g_returnSavedGame;
static void* g_namesAddress;
static const char* g_constructedText;
static long g_constructedLength;
static void* g_stringAddress;
static void* g_setNameAddress;
static void* g_addedProfileName;
static void* g_destroyedNamesAddress;

static void AddEvent(Event event)
{
    g_events[g_eventCount++] = event;
}

extern "C" void* __cdecl
FableFrontEndLoadProfileGetService()
{
    AddEvent(kGetService);
    ++g_serviceCalls;
    return &g_service;
}

extern "C" void __fastcall
FableFrontEndLoadProfileLoadSelected(
    void* service,
    void*,
    void* profileName,
    long activate)
{
    AddEvent(kLoadSelected);
    if (service == &g_service)
    {
        g_loadedProfileName = profileName;
        g_activate = activate;
    }
}

extern "C" void __fastcall
FableFrontEndLoadProfileGetSavedGameNames(
    void* manager,
    void*,
    void* names)
{
    AddEvent(kGetSavedGameNames);
    g_namesAddress = names;
    NamesProbe* probe = (NamesProbe*)names;
    if (manager == g_expectedManager && g_returnSavedGame)
    {
        probe->first = (void*)0x1000;
        probe->last = (void*)0x1004;
        probe->end = (void*)0x1004;
    }
}

extern "C" void __fastcall
FableFrontEndLoadProfileConstructString(
    void* string,
    void*,
    const char* text,
    long length)
{
    AddEvent(kConstructString);
    g_stringAddress = string;
    g_constructedText = text;
    g_constructedLength = length;
    ((StringProbe*)string)->text = text;
}

extern "C" void __fastcall
FableFrontEndLoadProfileSetMainMenuDefName(
    void* manager,
    void*,
    void* name)
{
    AddEvent(kSetMainMenuDefName);
    if (manager == g_expectedManager)
        g_setNameAddress = name;
}

extern "C" void __fastcall
FableFrontEndLoadProfileDestroyString(void* string, void*)
{
    AddEvent(kDestroyString);
    if (string == g_stringAddress)
        ((StringProbe*)string)->text = 0;
}

extern "C" void __fastcall
FableFrontEndLoadProfileAddProfileNameToScreens(
    void* manager,
    void*,
    void* profileName)
{
    AddEvent(kAddProfileName);
    if (manager == g_expectedManager)
        g_addedProfileName = profileName;
}

extern "C" void __fastcall
FableFrontEndLoadProfileDestroyNames(void* names, void*)
{
    AddEvent(kDestroyNames);
    g_destroyedNamesAddress = names;
}

static void Reset(
    unsigned char* manager,
    void* profileName,
    bool returnSavedGame)
{
    std::memset(manager, 0, 0x20);
    std::memset(&g_service, 0xff, sizeof(g_service));
    g_eventCount = 0;
    g_expectedManager = manager;
    g_expectedProfileName = profileName;
    g_serviceCalls = 0;
    g_loadedProfileName = 0;
    g_activate = 0;
    g_returnSavedGame = returnSavedGame;
    g_namesAddress = 0;
    g_constructedText = 0;
    g_constructedLength = 0;
    g_stringAddress = 0;
    g_setNameAddress = 0;
    g_addedProfileName = 0;
    g_destroyedNamesAddress = 0;
}

static bool RunCase(bool returnSavedGame, const char* expectedDefinition)
{
    unsigned char manager[0x20];
    unsigned long profileName[2] = { 0x12345678, 0x87654321 };
    Reset(manager, profileName, returnSavedGame);

    CFrontEndManager_LoadProfile_0059899a(manager, 0, profileName);

    static const Event expectedEvents[] =
    {
        kGetService,
        kGetService,
        kLoadSelected,
        kGetSavedGameNames,
        kConstructString,
        kSetMainMenuDefName,
        kDestroyString,
        kAddProfileName,
        kDestroyNames
    };
    const bool eventOrder =
        g_eventCount == sizeof(expectedEvents) / sizeof(expectedEvents[0]) &&
        std::memcmp(
            g_events,
            expectedEvents,
            sizeof(expectedEvents)) == 0;

    return eventOrder &&
        g_serviceCalls == 2 &&
        g_service.bytes[0x0c] == 0 &&
        g_loadedProfileName == g_expectedProfileName &&
        g_activate == 1 &&
        g_namesAddress != 0 &&
        g_destroyedNamesAddress == g_namesAddress &&
        g_stringAddress != 0 &&
        g_setNameAddress == g_stringAddress &&
        g_constructedLength == -1 &&
        std::strcmp(g_constructedText, expectedDefinition) == 0 &&
        g_addedProfileName == g_expectedProfileName;
}

int main()
{
    const bool empty = RunCase(
        false,
        FableFrontEndLoadProfileNoLiveAwareNoContinue);
    const bool populated = RunCase(
        true,
        FableFrontEndLoadProfileNoLiveAware);
    if (!empty || !populated)
    {
        std::printf(
            "FSE2_0059899a_TEST FAIL empty=%d populated=%d\n",
            empty ? 1 : 0,
            populated ? 1 : 0);
        return 1;
    }
    std::puts("FSE2_0059899a_TEST PASS");
    return 0;
}
