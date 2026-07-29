#include <cstdio>

// Standalone behavior fixture for
// CGameScriptInterface::TellHeroQuestObjectiveCompleted @ 0x00891800.
struct CQuestManager
{
    void SetObjectiveAsCompleted(unsigned long objectiveId);
};

static void* g_managerThis;
static unsigned long g_objectiveId;
static unsigned long g_objectiveState;
static int g_callCount;

CQuestManager* DAT_013b89fc;

void CQuestManager::SetObjectiveAsCompleted(unsigned long objectiveId)
{
    g_managerThis = this;
    g_objectiveId = objectiveId;
    g_objectiveState = 1;
    ++g_callCount;
}

void __fastcall CGameScriptInterface_TellHeroQuestObjectiveCompleted(
    void*,
    unsigned long,
    unsigned long objectiveId)
{
    DAT_013b89fc->SetObjectiveAsCompleted(objectiveId);
}

static int CheckCall(
    CQuestManager* manager,
    unsigned long objectiveId)
{
    g_managerThis = 0;
    g_objectiveId = 0;
    g_objectiveState = 0;
    g_callCount = 0;
    DAT_013b89fc = manager;

    CGameScriptInterface_TellHeroQuestObjectiveCompleted(
        (void*)0x12345678,
        0,
        objectiveId);

    if (g_callCount != 1 || g_managerThis != manager)
    {
        std::printf("quest manager dispatch failed\n");
        return 1;
    }
    if (g_objectiveId != objectiveId || g_objectiveState != 1)
    {
        std::printf("completed objective forwarding failed\n");
        return 1;
    }
    return 0;
}

int main()
{
    int failures = 0;
    CQuestManager manager;

    failures += CheckCall(&manager, 0);
    failures += CheckCall(&manager, 0xFEDCBA98UL);

    if (failures != 0)
    {
        std::printf("FSE2_00891800_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("FSE2_00891800_TEST PASS\n");
    return 0;
}
