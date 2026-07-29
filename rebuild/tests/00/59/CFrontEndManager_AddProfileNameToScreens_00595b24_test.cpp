// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::AddProfileNameToScreens @ 0x00595B24.

#include <cstdio>
#include <cstring>

extern "C" void __fastcall
CFrontEndManager_AddProfileNameToScreens_00595b24(
    void* manager,
    void*,
    const void* profileName);

extern "C" const unsigned char
FableFrontEndProfileTitlesSeparator[] = { 0x20, 0 };
extern "C" const char
FableFrontEndProfileTitlesNewGame[] = "UI_TEXT_NEW_GAME";
extern "C" const char
FableFrontEndProfileTitlesLoadGame[] = "UI_TEXT_LOAD_GAME";
extern "C" const char
FableFrontEndProfileTitlesOptions[] = "UI_TEXT_OPTIONS_MENU_TITLE";
extern "C" const char
FableFrontEndProfileTitlesGameOptions[] =
    "UI_TEXT_GAME_OPTIONS_MENU_TITLE";
extern "C" const char
FableFrontEndProfileTitlesVideo[] = "UI_TEXT_VIDEO_MENU_TITLE";
extern "C" const char
FableFrontEndProfileTitlesScoreboard[] =
    "UI_TEXT_SCOREBOARD_MENU_TITLE";
extern "C" const char
FableFrontEndProfileTitlesRedefineKeys[] =
    "UI_TEXT_REDEFINE_KEYS_MENU_TITLE";
extern "C" const char
FableFrontEndProfileTitlesAudio[] =
    "UI_TEXT_AUDIO_OPTIONS_MENU_TITLE";

struct NameProbe
{
    const char* text;
};

struct AppliedTitle
{
    unsigned long screen;
    const char* text;
    void* prefixAddress;
};

enum Event
{
    kPrepare = 1,
    kConstruct,
    kApply,
    kDestroyName,
    kDestroyPrefix
};

static Event g_events[32];
static int g_eventCount;
static void* g_expectedManager;
static const void* g_originalProfile;
static const void* g_preparedOriginal;
static const void* g_preparedSeparator;
static void* g_prefixAddress;
static unsigned char g_prefixMarker;
static int g_constructCalls;
static long g_lengths[9];
static AppliedTitle g_applied[9];
static int g_applyCalls;
static int g_destroyNameCalls;
static void* g_nameAddress;
static void* g_destroyedPrefix;

static void AddEvent(Event event)
{
    g_events[g_eventCount++] = event;
}

extern "C" void __fastcall
FableFrontEndProfileTitlesPreparePrefix(
    void* destination,
    const void* profileName,
    const void* separator)
{
    AddEvent(kPrepare);
    g_preparedOriginal = profileName;
    g_preparedSeparator = separator;
    g_prefixAddress = destination;
    *(void**)destination = &g_prefixMarker;
}

extern "C" void __fastcall
FableFrontEndProfileTitlesConstructName(
    void* name,
    void*,
    const char* text,
    long length)
{
    AddEvent(kConstruct);
    g_nameAddress = name;
    g_lengths[g_constructCalls++] = length;
    ((NameProbe*)name)->text = text;
}

extern "C" void __fastcall
FableFrontEndProfileTitlesAddPrefix(
    void* manager,
    void*,
    unsigned long screen,
    void* titleName,
    void* prefix)
{
    AddEvent(kApply);
    if (manager == g_expectedManager)
    {
        g_applied[g_applyCalls].screen = screen;
        g_applied[g_applyCalls].text = ((NameProbe*)titleName)->text;
        g_applied[g_applyCalls].prefixAddress = prefix;
        ++g_applyCalls;
    }
}

extern "C" void __fastcall
FableFrontEndProfileTitlesDestroyName(void* name, void*)
{
    AddEvent(kDestroyName);
    if (name == g_nameAddress)
        ++g_destroyNameCalls;
}

extern "C" void __fastcall
FableFrontEndProfileTitlesDestroyPrefix(void* prefix, void*)
{
    AddEvent(kDestroyPrefix);
    g_destroyedPrefix = prefix;
}

int main()
{
    unsigned char manager[0x20];
    unsigned long profileName[2] = { 0x11223344, 0x55667788 };
    std::memset(manager, 0, sizeof(manager));
    g_eventCount = 0;
    g_expectedManager = manager;
    g_originalProfile = profileName;
    g_preparedOriginal = 0;
    g_preparedSeparator = 0;
    g_prefixAddress = 0;
    g_constructCalls = 0;
    std::memset(g_lengths, 0, sizeof(g_lengths));
    std::memset(g_applied, 0, sizeof(g_applied));
    g_applyCalls = 0;
    g_destroyNameCalls = 0;
    g_nameAddress = 0;
    g_destroyedPrefix = 0;

    CFrontEndManager_AddProfileNameToScreens_00595b24(
        manager, 0, profileName);

    static const unsigned long expectedScreens[] =
    {
        0, 0, 0x18, 1, 1, 5, 0x19, 0x16, 4
    };
    static const char* expectedNames[] =
    {
        FableFrontEndProfileTitlesNewGame,
        FableFrontEndProfileTitlesLoadGame,
        FableFrontEndProfileTitlesOptions,
        FableFrontEndProfileTitlesOptions,
        FableFrontEndProfileTitlesGameOptions,
        FableFrontEndProfileTitlesVideo,
        FableFrontEndProfileTitlesScoreboard,
        FableFrontEndProfileTitlesRedefineKeys,
        FableFrontEndProfileTitlesAudio
    };
    bool mappings = true;
    for (int i = 0; i < 9; ++i)
    {
        mappings = mappings &&
            g_lengths[i] == -1 &&
            g_applied[i].screen == expectedScreens[i] &&
            std::strcmp(g_applied[i].text, expectedNames[i]) == 0 &&
            g_applied[i].prefixAddress == g_prefixAddress;
    }
    bool lifetimeOrder =
        g_eventCount == 29 &&
        g_events[0] == kPrepare &&
        g_events[28] == kDestroyPrefix;
    for (int i = 0; i < 9; ++i)
    {
        const int base = 1 + i * 3;
        lifetimeOrder = lifetimeOrder &&
            g_events[base] == kConstruct &&
            g_events[base + 1] == kApply &&
            g_events[base + 2] == kDestroyName;
    }
    const bool pass =
        lifetimeOrder &&
        mappings &&
        g_preparedOriginal == g_originalProfile &&
        g_preparedSeparator == FableFrontEndProfileTitlesSeparator &&
        g_constructCalls == 9 &&
        g_applyCalls == 9 &&
        g_destroyNameCalls == 9 &&
        g_destroyedPrefix == g_prefixAddress;
    if (!pass)
    {
        std::printf(
            "FSE2_00595b24_TEST FAIL order=%d mappings=%d "
            "construct=%d apply=%d destroy=%d prefix=%d\n",
            lifetimeOrder ? 1 : 0,
            mappings ? 1 : 0,
            g_constructCalls,
            g_applyCalls,
            g_destroyNameCalls,
            g_destroyedPrefix == g_prefixAddress ? 1 : 0);
        return 1;
    }
    std::puts("FSE2_00595b24_TEST PASS");
    return 0;
}
