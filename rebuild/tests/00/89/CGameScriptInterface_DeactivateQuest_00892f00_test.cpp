#include <cstdio>

// Standalone behavior fixture for
// CGameScriptInterface::DeactivateQuest @ 0x00892f00.
struct CCharString
{
    const char* text;
};

struct CQuestManager
{
    void DeactivateQuest(
        const CCharString& questName,
        unsigned long flags);
};

static void* g_managerThis;
static const CCharString* g_questNameArgument;
static unsigned long g_flagsArgument;
static int g_callCount;

CQuestManager* DAT_013b89fc;

void CQuestManager::DeactivateQuest(
    const CCharString& questName,
    unsigned long flags)
{
    g_managerThis = this;
    g_questNameArgument = &questName;
    g_flagsArgument = flags;
    ++g_callCount;
}

void __fastcall CGameScriptInterface_DeactivateQuest(
    void*,
    unsigned long,
    const CCharString& questName,
    unsigned long flags)
{
    DAT_013b89fc->DeactivateQuest(questName, flags);
}

static int CheckCall(
    CQuestManager* manager,
    CCharString* questName,
    unsigned long flags)
{
    g_managerThis = 0;
    g_questNameArgument = 0;
    g_flagsArgument = 0;
    g_callCount = 0;
    DAT_013b89fc = manager;

    CGameScriptInterface_DeactivateQuest(
        (void*)0x12345678,
        0,
        *questName,
        flags);

    if (g_callCount != 1 || g_managerThis != manager)
    {
        std::printf("quest manager dispatch failed\n");
        return 1;
    }
    if (g_questNameArgument != questName || g_flagsArgument != flags)
    {
        std::printf("deactivation argument forwarding failed\n");
        return 1;
    }
    return 0;
}

int main()
{
    int failures = 0;
    CQuestManager manager;
    CCharString questName;
    questName.text = "QST_CARD_TRADER_01";

    failures += CheckCall(&manager, &questName, 0);
    failures += CheckCall(&manager, &questName, 0x80000001UL);

    if (failures != 0)
    {
        std::printf("FSE2_00892f00_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("FSE2_00892f00_TEST PASS\n");
    return 0;
}
