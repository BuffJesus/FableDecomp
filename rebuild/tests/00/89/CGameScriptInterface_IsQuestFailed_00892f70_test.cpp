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
    bool IsQuestFailed(const CCharString& questName);
};

class CGameScriptInterface
{
public:
    virtual bool IsQuestFailed(
        const CCharString& questName) const;
};

CQuestManager* CGSIIsQuestFailed_Manager;
static CQuestManager* g_ObservedManager;
static const CCharString* g_ObservedQuestName;
static bool g_Result;
static unsigned long g_CallCount;

bool CQuestManager::IsQuestFailed(const CCharString& questName)
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
    CGSIIsQuestFailed_Manager = &manager;

    g_Result = false;
    if (interfaceObject.IsQuestFailed(firstQuest))
        return 1;
    if (g_CallCount != 1 || g_ObservedManager != &manager ||
        g_ObservedQuestName != &firstQuest)
        return 2;

    g_Result = true;
    if (!interfaceObject.IsQuestFailed(secondQuest))
        return 3;
    if (g_CallCount != 2 || g_ObservedManager != &manager ||
        g_ObservedQuestName != &secondQuest)
        return 4;

    std::printf("FSE2_00892f70_TEST PASS\n");
    return 0;
}
