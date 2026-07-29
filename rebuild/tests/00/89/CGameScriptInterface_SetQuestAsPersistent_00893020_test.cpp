#include <cstdio>

// Standalone behavior fixture for
// CGameScriptInterface::SetQuestAsPersistent @ 0x00893020.
struct CCharString
{
    const char* text;
};

struct CQuestManager
{
    void SetQuestAsPersistent(
        const CCharString& questName,
        bool persistent);
};

static void* g_managerThis;
static const CCharString* g_questNameArgument;
static bool g_persistentArgument;
static int g_callCount;

CQuestManager* DAT_013b89fc;

void CQuestManager::SetQuestAsPersistent(
    const CCharString& questName,
    bool persistent)
{
    g_managerThis = this;
    g_questNameArgument = &questName;
    g_persistentArgument = persistent;
    ++g_callCount;
}

void __fastcall CGameScriptInterface_SetQuestAsPersistent(
    void*,
    unsigned long,
    const CCharString& questName,
    bool persistent)
{
    DAT_013b89fc->SetQuestAsPersistent(questName, persistent);
}

static int CheckCall(
    CQuestManager* manager,
    CCharString* questName,
    bool persistent)
{
    g_managerThis = 0;
    g_questNameArgument = 0;
    g_persistentArgument = !persistent;
    g_callCount = 0;
    DAT_013b89fc = manager;

    CGameScriptInterface_SetQuestAsPersistent(
        (void*)0x12345678,
        0,
        *questName,
        persistent);

    if (g_callCount != 1 || g_managerThis != manager)
    {
        std::printf("quest manager dispatch failed\n");
        return 1;
    }
    if (g_questNameArgument != questName ||
        g_persistentArgument != persistent)
    {
        std::printf("argument forwarding failed\n");
        return 1;
    }
    return 0;
}

int main()
{
    int failures = 0;
    CQuestManager manager;
    CCharString questName;
    questName.text = "QST_HEROES_GUILD";

    failures += CheckCall(&manager, &questName, false);
    failures += CheckCall(&manager, &questName, true);

    if (failures != 0)
    {
        std::printf("FSE2_00893020_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("FSE2_00893020_TEST PASS\n");
    return 0;
}
