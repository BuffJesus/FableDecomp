// Real-object behavior harness for
// CGameScriptInterface::DeactivateQuestLater @ 0x00892F10.

#include <cstdio>

struct CCharString
{
    const char* text;
};

struct CQuestManager
{
    void DeactivateQuestLater(
        const CCharString& questName,
        unsigned long delay);
};

CQuestManager* FableGlobalQuestManager;

static CQuestManager* g_seenManager;
static const CCharString* g_seenQuestName;
static unsigned long g_seenDelay;
static int g_callCount;

void CQuestManager::DeactivateQuestLater(
    const CCharString& questName,
    unsigned long delay)
{
    g_seenManager = this;
    g_seenQuestName = &questName;
    g_seenDelay = delay;
    ++g_callCount;
}

void __fastcall CGameScriptInterface_DeactivateQuestLater(
    const void* self,
    void*,
    const CCharString& questName,
    unsigned long delay);

static int RunCase(
    const void* ignoredSelf,
    CQuestManager& manager,
    const CCharString& questName,
    unsigned long delay)
{
    FableGlobalQuestManager = &manager;
    g_seenManager = 0;
    g_seenQuestName = 0;
    g_seenDelay = 0;
    g_callCount = 0;

    CGameScriptInterface_DeactivateQuestLater(
        ignoredSelf,
        0,
        questName,
        delay);

    return
        g_callCount == 1 &&
        g_seenManager == &manager &&
        g_seenQuestName == &questName &&
        g_seenDelay == delay;
}

int main()
{
    CQuestManager firstManager;
    CQuestManager secondManager;
    CCharString firstQuest = { "Q_First" };
    CCharString secondQuest = { "Q_Second" };

    if (!RunCase(
            (const void*)0x11111111,
            firstManager,
            firstQuest,
            0))
        return 1;

    if (!RunCase(
            (const void*)0xeeeeeeee,
            secondManager,
            secondQuest,
            0xffffffff))
        return 2;

    std::printf("FSE2_00892f10_TEST PASS\n");
    return 0;
}
