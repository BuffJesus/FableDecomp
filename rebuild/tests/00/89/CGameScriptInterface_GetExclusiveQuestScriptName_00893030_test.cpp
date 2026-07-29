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
    const CCharString& GetActiveExclusiveQuestName();

    const CCharString* m_Result;
};

class CGameScriptInterface
{
public:
    virtual const CCharString& GetExclusiveQuestScriptName() const;
};

CQuestManager* CGSIGetExclusiveQuestScriptName_Manager;
static CQuestManager* g_ObservedManager;
static unsigned long g_CallCount;

const CCharString& CQuestManager::GetActiveExclusiveQuestName()
{
    g_ObservedManager = this;
    ++g_CallCount;
    return *m_Result;
}

int main()
{
    CCharString firstName(0x11223344);
    CCharString secondName(0x55667788);
    CQuestManager manager;
    CGameScriptInterface interfaceObject;
    CGSIGetExclusiveQuestScriptName_Manager = &manager;

    manager.m_Result = &firstName;
    const CCharString& firstResult =
        interfaceObject.GetExclusiveQuestScriptName();
    if (g_CallCount != 1 || g_ObservedManager != &manager ||
        &firstResult != &firstName)
        return 1;

    manager.m_Result = &secondName;
    const CCharString& secondResult =
        interfaceObject.GetExclusiveQuestScriptName();
    if (g_CallCount != 2 || g_ObservedManager != &manager ||
        &secondResult != &secondName)
        return 2;

    std::printf("FSE2_00893030_TEST PASS\n");
    return 0;
}
