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
    bool IsQuestRegistered(const CCharString& questName);
};

class CGameScriptInterface
{
public:
    virtual bool IsQuestRegistered(const CCharString& questName) const;
};

CQuestManager* CGSIIsQuestRegistered_Manager;
static const CCharString* g_ObservedQuestName;
static unsigned long g_CallCount;
static bool g_Result;

bool CQuestManager::IsQuestRegistered(const CCharString& questName)
{
    g_ObservedQuestName = &questName;
    ++g_CallCount;
    return g_Result;
}

int main()
{
    CQuestManager manager;
    CGameScriptInterface interfaceObject;
    CCharString firstQuest(0x10203040);
    CCharString secondQuest(0x50607080);
    CGSIIsQuestRegistered_Manager = &manager;

    g_Result = false;
    if (interfaceObject.IsQuestRegistered(firstQuest))
        return 1;
    if (g_CallCount != 1 || g_ObservedQuestName != &firstQuest)
        return 2;

    g_Result = true;
    if (!interfaceObject.IsQuestRegistered(secondQuest))
        return 3;
    if (g_CallCount != 2 || g_ObservedQuestName != &secondQuest)
        return 4;

    std::printf("FSE2_00892f50_TEST PASS\n");
    return 0;
}
