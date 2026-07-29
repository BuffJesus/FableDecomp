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
    bool IsQuestActive(const CCharString& questName);
};

class CGameScriptInterface
{
public:
    virtual bool IsQuestActive(
        const CCharString& questName) const;
};

CQuestManager* CGSIIsQuestActive_Manager;
static CQuestManager* g_ObservedManager;
static const CCharString* g_ObservedQuestName;
static bool g_Result;
static unsigned long g_CallCount;

bool CQuestManager::IsQuestActive(const CCharString& questName)
{
    g_ObservedManager = this;
    g_ObservedQuestName = &questName;
    ++g_CallCount;
    return g_Result;
}

int main()
{
    CQuestManager manager;
    CGameScriptInterface interfaceObject;
    CCharString firstQuest(0x11223344);
    CCharString secondQuest(0x55667788);
    CGSIIsQuestActive_Manager = &manager;

    g_Result = false;
    if (interfaceObject.IsQuestActive(firstQuest))
        return 1;
    if (g_CallCount != 1 || g_ObservedManager != &manager ||
        g_ObservedQuestName != &firstQuest)
        return 2;

    g_Result = true;
    if (!interfaceObject.IsQuestActive(secondQuest))
        return 3;
    if (g_CallCount != 2 || g_ObservedManager != &manager ||
        g_ObservedQuestName != &secondQuest)
        return 4;

    std::printf("FSE2_00892f40_TEST PASS\n");
    return 0;
}
