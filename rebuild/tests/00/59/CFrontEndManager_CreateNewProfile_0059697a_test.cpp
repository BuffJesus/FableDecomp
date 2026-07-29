// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::CreateNewProfile @ 0x0059697A.

#include <cstdio>
#include <cstring>

struct FrontEndManagerProbe
{
    unsigned char bytes[0xa4];
};

struct ProfileManager
{
    unsigned char bytes[0xe0];
};

struct WideString
{
    void* data;
};

struct CharString
{
    void* data;
};

extern "C" unsigned char __fastcall
CFrontEndManager_CreateNewProfile_0059697a(
    void* manager,
    void*,
    void* name);

extern "C" const char
FableFrontEndCreateProfileNoContinueMenuName[] =
    "UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE_NO_CONTINUE";

enum Event
{
    kHasSpace = 1,
    kDefaultValue,
    kLength,
    kDestroyDefault,
    kAssignName,
    kSetDefaults,
    kApplyValues,
    kSetInverted,
    kSave,
    kConstructLiteral,
    kSetMenu,
    kDestroyLiteral,
    kAddName
};

static Event g_events[24];
static int g_eventCount;
static ProfileManager g_profile;
static void* g_expectedManager;
static WideString* g_expectedName;
static unsigned char g_hasSpace;
static long g_defaultLength;
static bool g_applySelectsAlternate;
static int g_getProfileManagerCalls;
static void* g_defaultLocal;
static void* g_destroyedDefault;
static void* g_assignedName;
static int g_assignOption;
static int g_setDefaultsCalls;
static int g_applyCalls;
static unsigned char g_invertedLow;
static int g_invertedApply;
static int g_saveCalls;
static void* g_literalLocal;
static const char* g_literal;
static int g_literalLength;
static void* g_menuText;
static void* g_destroyedLiteral;
static void* g_addedName;

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

extern "C" void* __cdecl
FableFrontEndCreateProfileGetProfileManager()
{
    ++g_getProfileManagerCalls;
    return &g_profile;
}

extern "C" unsigned char __fastcall
FableFrontEndCreateProfileHasSpace(void* profileManager, void*)
{
    AddEvent(kHasSpace);
    return profileManager == &g_profile ? g_hasSpace : 0;
}

extern "C" void* __fastcall
FableFrontEndCreateProfileGetDefaultValue(
    void* profileManager,
    void*,
    void* result,
    void* name,
    int create)
{
    AddEvent(kDefaultValue);
    g_defaultLocal = result;
    CharString* text = (CharString*)result;
    if (profileManager == &g_profile &&
        name == g_expectedName &&
        create == 1)
    {
        text->data = (void*)0x11112222;
    }
    else
    {
        text->data = 0;
    }
    return result;
}

extern "C" long __fastcall
FableFrontEndCreateProfileStringLength(void* text, void*)
{
    AddEvent(kLength);
    return text == g_defaultLocal ? g_defaultLength : 0;
}

extern "C" void __fastcall
FableFrontEndCreateProfileDestroyString(void* text, void*)
{
    AddEvent(kDestroyDefault);
    g_destroyedDefault = text;
}

extern "C" void __fastcall
FableFrontEndCreateProfileAssignName(
    void* profileManager,
    void*,
    void* name,
    int option)
{
    AddEvent(kAssignName);
    if (profileManager == &g_profile)
    {
        g_assignedName = name;
        g_assignOption = option;
    }
}

extern "C" void __fastcall
FableFrontEndCreateProfileSetDefaults(void* profileManager, void*)
{
    AddEvent(kSetDefaults);
    if (profileManager == &g_profile)
        ++g_setDefaultsCalls;
}

extern "C" void __fastcall
FableFrontEndCreateProfileApplyValues(void* profileManager, void*)
{
    AddEvent(kApplyValues);
    if (profileManager == &g_profile)
    {
        ++g_applyCalls;
        if (g_applySelectsAlternate)
            g_profile.bytes[0x0c] = 1;
    }
}

extern "C" void __fastcall
FableFrontEndCreateProfileSetBowInverted(
    void* profileManager,
    void*,
    int inverted,
    int apply)
{
    AddEvent(kSetInverted);
    if (profileManager == &g_profile)
    {
        g_invertedLow = (unsigned char)inverted;
        g_invertedApply = apply;
    }
}

extern "C" void __fastcall
FableFrontEndCreateProfileSave(void* profileManager, void*)
{
    AddEvent(kSave);
    if (profileManager == &g_profile)
        ++g_saveCalls;
}

extern "C" void __fastcall
FableFrontEndCreateProfileConstructLiteral(
    void* text,
    void*,
    const char* literal,
    int length)
{
    AddEvent(kConstructLiteral);
    g_literalLocal = text;
    g_literal = literal;
    g_literalLength = length;
    ((CharString*)text)->data = (void*)literal;
}

extern "C" void __fastcall
FableFrontEndCreateProfileSetMainMenuName(
    void* manager,
    void*,
    void* text)
{
    AddEvent(kSetMenu);
    if (manager == g_expectedManager)
        g_menuText = text;
}

extern "C" void __fastcall
FableFrontEndCreateProfileDestroyLiteral(void* text, void*)
{
    AddEvent(kDestroyLiteral);
    g_destroyedLiteral = text;
}

extern "C" void __fastcall
FableFrontEndCreateProfileAddNameToScreens(
    void* manager,
    void*,
    void* name)
{
    AddEvent(kAddName);
    if (manager == g_expectedManager)
        g_addedName = name;
}

static void Reset(
    FrontEndManagerProbe& manager,
    WideString& name)
{
    std::memset(&manager, 0, sizeof(manager));
    std::memset(&g_profile, 0, sizeof(g_profile));
    name.data = (void*)0x55667788;
    g_profile.bytes[0x0c] = 1;
    g_profile.bytes[0x10] = 0x31;
    g_profile.bytes[0xd0] = 0x42;
    ManagerPointer(manager, 0x60) = (void*)0x60606060;
    ManagerPointer(manager, 0x98) = (void*)0x98989898;
    ManagerPointer(manager, 0x9c) = (void*)0x9c9c9c9c;
    g_expectedManager = &manager;
    g_expectedName = &name;
    g_hasSpace = 1;
    g_defaultLength = 8;
    g_applySelectsAlternate = false;
    g_eventCount = 0;
    g_getProfileManagerCalls = 0;
    g_defaultLocal = 0;
    g_destroyedDefault = 0;
    g_assignedName = 0;
    g_assignOption = -1;
    g_setDefaultsCalls = 0;
    g_applyCalls = 0;
    g_invertedLow = 0;
    g_invertedApply = 0;
    g_saveCalls = 0;
    g_literalLocal = 0;
    g_literal = 0;
    g_literalLength = 0;
    g_menuText = 0;
    g_destroyedLiteral = 0;
    g_addedName = 0;
}

static int CheckNoSpace()
{
    FrontEndManagerProbe manager;
    WideString name;
    Reset(manager, name);
    g_hasSpace = 0;

    const unsigned char result =
        CFrontEndManager_CreateNewProfile_0059697a(
            &manager,
            0,
            &name);

    if (result != 0 ||
        g_eventCount != 1 ||
        g_events[0] != kHasSpace ||
        g_getProfileManagerCalls != 1)
        return 1;
    if (g_profile.bytes[0x0c] != 1 ||
        g_assignedName != 0 ||
        g_saveCalls != 0)
        return 2;
    return 0;
}

static int CheckCreation(
    long defaultLength,
    bool alternateInversion)
{
    FrontEndManagerProbe manager;
    WideString name;
    Reset(manager, name);
    g_defaultLength = defaultLength;
    g_applySelectsAlternate = alternateInversion;

    const unsigned char result =
        CFrontEndManager_CreateNewProfile_0059697a(
            &manager,
            0,
            &name);

    if (result != (defaultLength != 0 ? 1 : 0))
        return 10;
    if (g_getProfileManagerCalls != 9 ||
        g_profile.bytes[0x0c] !=
            (alternateInversion ? 1 : 0))
        return 11;
    if (g_defaultLocal == 0 ||
        g_destroyedDefault != g_defaultLocal ||
        g_assignedName != &name ||
        g_assignOption != 0)
        return 12;
    if (g_setDefaultsCalls != 1 ||
        g_applyCalls != 1 ||
        g_invertedLow !=
            (alternateInversion ? 0x42 : 0x31) ||
        g_invertedApply != 1 ||
        g_saveCalls != 1)
        return 13;
    if (g_literalLocal == 0 ||
        g_menuText != g_literalLocal ||
        g_destroyedLiteral != g_literalLocal ||
        g_literal !=
            FableFrontEndCreateProfileNoContinueMenuName ||
        std::strcmp(
            g_literal,
            "UI_FRONTEND_MAIN_MENU_NO_LIVEAWARE_NO_CONTINUE") != 0 ||
        g_literalLength != -1)
        return 14;
    if (g_addedName != &name)
        return 15;
    if (ManagerPointer(manager, 0x60) != (void*)0x60606060 ||
        ManagerPointer(manager, 0x98) != (void*)0x98989898 ||
        ManagerPointer(manager, 0x9c) != (void*)0x9c9c9c9c)
        return 16;
    if (g_events[g_eventCount - 1] != kAddName)
        return 17;
    return 0;
}

int main()
{
    int failure = CheckNoSpace();
    if (failure != 0)
        return failure;
    failure = CheckCreation(12, false);
    if (failure != 0)
        return failure;
    failure = CheckCreation(0, true);
    if (failure != 0)
        return 20 + failure;

    std::printf("FSE2_0059697a_TEST PASS\n");
    return 0;
}
