#include <cstdio>

class CCharString
{
public:
    explicit CCharString(unsigned long identity)
        : m_Identity(identity)
    {
    }

    unsigned long m_Identity;
};

class CQuestManager
{
public:
    void AddNewObjective(
        const CCharString& questScriptName,
        unsigned long objectiveId);
};

class CGameScriptInterface
{
public:
    virtual void GiveHeroNewQuestObjective(
        const CCharString& questScriptName,
        unsigned long objectiveId) const;
};

CQuestManager* CGSIGiveHeroNewQuestObjective_Manager;
static CQuestManager* g_ObservedManager;
static const CCharString* g_ObservedQuestScriptName;
static unsigned long g_ObservedObjectiveId;
static unsigned long g_CallCount;

void CQuestManager::AddNewObjective(
    const CCharString& questScriptName,
    unsigned long objectiveId)
{
    g_ObservedManager = this;
    g_ObservedQuestScriptName = &questScriptName;
    g_ObservedObjectiveId = objectiveId;
    ++g_CallCount;
}

int main()
{
    CQuestManager manager;
    CGameScriptInterface interfaceObject;
    CCharString firstQuest(0x11223344);
    CCharString secondQuest(0x55667788);
    CGSIGiveHeroNewQuestObjective_Manager = &manager;

    interfaceObject.GiveHeroNewQuestObjective(firstQuest, 0);
    if (g_CallCount != 1 || g_ObservedManager != &manager ||
        g_ObservedQuestScriptName != &firstQuest ||
        g_ObservedObjectiveId != 0)
        return 1;

    interfaceObject.GiveHeroNewQuestObjective(secondQuest, 0xFEDCBA98UL);
    if (g_CallCount != 2 || g_ObservedManager != &manager ||
        g_ObservedQuestScriptName != &secondQuest ||
        g_ObservedObjectiveId != 0xFEDCBA98UL)
        return 2;

    std::printf("FSE2_008917f0_TEST PASS\n");
    return 0;
}
