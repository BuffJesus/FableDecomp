// Focused profile-list construction fixture for
// NUISystem::CFrontEndManager::RefreshAvailableProfiles @ 0x00596CCE.

#include <cstdio>
#include <cstring>

struct FableRefreshProfilesWideString
{
    void* data;
};

struct FableRefreshProfilesNameVector
{
    FableRefreshProfilesWideString* begin;
    FableRefreshProfilesWideString* end;
    FableRefreshProfilesWideString* capacity;
};

extern "C" long __fastcall
CFrontEndManager_RefreshAvailableProfiles_00596cce(
    void* manager,
    void*);

enum Event
{
    kGetNames = 1,
    kBeginList,
    kCreateButton,
    kVerify,
    kCreateAction,
    kAttach,
    kAppend,
    kReleaseAction,
    kReleaseButton,
    kFinish,
    kDestroyNames
};

struct ButtonRecord
{
    const FableRefreshProfilesWideString* name;
    float y;
    unsigned char newProfile;
    void* action;
};

struct ActionRecord
{
    unsigned long id;
    const FableRefreshProfilesWideString* name;
};

static Event g_events[80];
static int g_eventCount;
static void* g_expectedManager;
static FableRefreshProfilesWideString g_names[4];
static long g_nameCount;
static unsigned char g_valid[4];
static ButtonRecord g_buttons[5];
static ActionRecord g_actions[5];
static int g_buttonCount;
static int g_actionCount;
static void* g_appended[5];
static int g_appendCount;
static int g_attachCount;
static int g_releaseActionCount;
static int g_releaseButtonCount;
static int g_finishCount;
static int g_destroyCount;
static unsigned long g_usedKey;
static void* g_list = (void*)0x13572468;
static FableRefreshProfilesNameVector* g_destroyedVector;

static void AddEvent(Event event)
{
    g_events[g_eventCount++] = event;
}

extern "C" void __fastcall
FableRefreshProfilesGetNames(
    void* manager,
    void*,
    FableRefreshProfilesNameVector* names)
{
    AddEvent(kGetNames);
    if (manager != g_expectedManager ||
        names->begin != 0 ||
        names->end != 0 ||
        names->capacity != 0)
    {
        names->begin = names->end = names->capacity = 0;
        return;
    }
    names->begin = g_names;
    names->end = g_names + g_nameCount;
    names->capacity = g_names + 4;
}

extern "C" void* __fastcall
FableRefreshProfilesBeginList(
    void* manager,
    void*,
    unsigned long usedKey)
{
    AddEvent(kBeginList);
    g_usedKey = manager == g_expectedManager
        ? usedKey
        : 0xffffffff;
    return g_list;
}

extern "C" void* __fastcall
FableRefreshProfilesCreateButton(
    void* manager,
    void*,
    const FableRefreshProfilesWideString* name,
    float y,
    unsigned char newProfile)
{
    AddEvent(kCreateButton);
    ButtonRecord& button = g_buttons[g_buttonCount++];
    button.name =
        manager == g_expectedManager
            ? name
            : (const FableRefreshProfilesWideString*)-1;
    button.y = y;
    button.newProfile = newProfile;
    button.action = 0;
    return &button;
}

extern "C" unsigned char __fastcall
FableRefreshProfilesVerifyName(
    void* manager,
    void*,
    const FableRefreshProfilesWideString* name)
{
    AddEvent(kVerify);
    if (manager != g_expectedManager ||
        name < g_names ||
        name >= g_names + g_nameCount)
        return 0;
    return g_valid[name - g_names];
}

extern "C" void* __fastcall
FableRefreshProfilesCreateAction(
    void* manager,
    void*,
    unsigned long actionId,
    const FableRefreshProfilesWideString* name)
{
    AddEvent(kCreateAction);
    ActionRecord& action = g_actions[g_actionCount++];
    action.id =
        manager == g_expectedManager ? actionId : 0xffffffff;
    action.name = name;
    return &action;
}

extern "C" void __fastcall
FableRefreshProfilesAttachAction(
    void* button,
    void*,
    void* action)
{
    AddEvent(kAttach);
    ++g_attachCount;
    ((ButtonRecord*)button)->action = action;
}

extern "C" void __fastcall
FableRefreshProfilesAppendButton(
    void* list,
    void*,
    void* button)
{
    AddEvent(kAppend);
    if (list == g_list)
        g_appended[g_appendCount++] = button;
}

extern "C" void __fastcall
FableRefreshProfilesReleaseAction(void*, void*)
{
    AddEvent(kReleaseAction);
    ++g_releaseActionCount;
}

extern "C" void __fastcall
FableRefreshProfilesReleaseButton(void*, void*)
{
    AddEvent(kReleaseButton);
    ++g_releaseButtonCount;
}

extern "C" void __fastcall
FableRefreshProfilesFinishList(void* list, void*)
{
    AddEvent(kFinish);
    if (list == g_list)
        ++g_finishCount;
}

extern "C" void __fastcall
FableRefreshProfilesDestroyNames(
    FableRefreshProfilesNameVector* names,
    void*)
{
    AddEvent(kDestroyNames);
    ++g_destroyCount;
    g_destroyedVector = names;
    if (names->begin != g_names ||
        names->end != g_names + g_nameCount)
        g_destroyCount = -100;
}

static void Reset(void* manager, long count)
{
    g_expectedManager = manager;
    g_nameCount = count;
    for (int i = 0; i != 4; ++i)
    {
        g_names[i].data = (void*)(0x1000 + i);
        g_valid[i] = 0;
    }
    g_eventCount = 0;
    std::memset(g_buttons, 0, sizeof(g_buttons));
    std::memset(g_actions, 0, sizeof(g_actions));
    std::memset(g_appended, 0, sizeof(g_appended));
    g_buttonCount = 0;
    g_actionCount = 0;
    g_appendCount = 0;
    g_attachCount = 0;
    g_releaseActionCount = 0;
    g_releaseButtonCount = 0;
    g_finishCount = 0;
    g_destroyCount = 0;
    g_usedKey = 0xffffffff;
    g_destroyedVector = 0;
}

static int CheckEmpty()
{
    int manager;
    Reset(&manager, 0);
    const long result =
        CFrontEndManager_RefreshAvailableProfiles_00596cce(
            &manager,
            0);
    if (result != 0 ||
        g_usedKey != 7 ||
        g_buttonCount != 1 ||
        g_actionCount != 1 ||
        g_actions[0].id != 0x125 ||
        g_actions[0].name != 0 ||
        g_buttons[0].name != 0 ||
        g_buttons[0].y != 0.0f ||
        g_buttons[0].newProfile != 1)
        return 1;
    if (g_appendCount != 1 ||
        g_attachCount != 1 ||
        g_releaseActionCount != 1 ||
        g_releaseButtonCount != 1 ||
        g_finishCount != 1 ||
        g_destroyCount != 1 ||
        g_destroyedVector == 0)
        return 2;
    if (g_events[g_eventCount - 4] != kFinish ||
        g_events[g_eventCount - 3] != kReleaseAction ||
        g_events[g_eventCount - 2] != kReleaseButton ||
        g_events[g_eventCount - 1] != kDestroyNames)
        return 3;
    return 0;
}

static int CheckProfiles()
{
    int manager;
    Reset(&manager, 3);
    g_valid[0] = 0;
    g_valid[1] = 1;
    g_valid[2] = 0;

    const long result =
        CFrontEndManager_RefreshAvailableProfiles_00596cce(
            &manager,
            0);
    if (result != 3 ||
        g_buttonCount != 4 ||
        g_actionCount != 4 ||
        g_appendCount != 4 ||
        g_attachCount != 4 ||
        g_releaseActionCount != 4 ||
        g_releaseButtonCount != 4)
        return 10;

    const unsigned long expectedActions[4] =
    {
        0x125, 0xdb, 0x124, 0xdb
    };
    const float expectedY[4] =
    {
        0.0f, 50.0f, 80.0f, 110.0f
    };
    for (int i = 0; i != 4; ++i)
    {
        if (g_actions[i].id != expectedActions[i] ||
            g_buttons[i].y != expectedY[i] ||
            g_appended[i] != &g_buttons[i] ||
            g_buttons[i].action != &g_actions[i])
            return 11 + i;
        if (i == 0)
        {
            if (g_buttons[i].newProfile != 1 ||
                g_buttons[i].name != 0 ||
                g_actions[i].name != 0)
                return 20;
        }
        else if (g_buttons[i].newProfile != 0 ||
                 g_buttons[i].name != &g_names[i - 1] ||
                 g_actions[i].name != &g_names[i - 1])
        {
            return 21 + i;
        }
    }
    if (g_finishCount != 1 || g_destroyCount != 1)
        return 30;

    Event expected[31];
    int expectedCount = 0;
    expected[expectedCount++] = kGetNames;
    expected[expectedCount++] = kBeginList;
    expected[expectedCount++] = kCreateButton;
    expected[expectedCount++] = kCreateAction;
    expected[expectedCount++] = kAttach;
    expected[expectedCount++] = kAppend;
    for (int i = 0; i != 3; ++i)
    {
        expected[expectedCount++] = kCreateButton;
        expected[expectedCount++] = kVerify;
        expected[expectedCount++] = kCreateAction;
        expected[expectedCount++] = kAttach;
        expected[expectedCount++] = kAppend;
        expected[expectedCount++] = kReleaseAction;
        expected[expectedCount++] = kReleaseButton;
    }
    expected[expectedCount++] = kFinish;
    expected[expectedCount++] = kReleaseAction;
    expected[expectedCount++] = kReleaseButton;
    expected[expectedCount++] = kDestroyNames;
    if (g_eventCount != expectedCount)
        return 31;
    for (int i = 0; i != expectedCount; ++i)
    {
        if (g_events[i] != expected[i])
            return 32;
    }
    return 0;
}

int main()
{
    int failure = CheckEmpty();
    if (failure != 0)
        return failure;
    failure = CheckProfiles();
    if (failure != 0)
        return failure;

    std::printf("FSE2_00596cce_TEST PASS\n");
    return 0;
}
