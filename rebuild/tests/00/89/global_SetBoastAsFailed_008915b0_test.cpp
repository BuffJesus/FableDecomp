#include <cstdio>

class CCharString
{
public:
    explicit CCharString(unsigned long value)
        : m_Value(value)
    {
    }

    CCharString(const CCharString& other)
        : m_Value(other.m_Value)
    {
    }

    unsigned long m_Value;
};

class CQuestManager
{
public:
    void SetBoastAsCompleted(
        long boastId,
        CCharString boastName)
    {
        g_lastBoastId = boastId;
        g_lastBoastValue = boastName.m_Value;
        g_callCount++;
    }

    static long g_lastBoastId;
    static unsigned long g_lastBoastValue;
    static int g_callCount;
};

long CQuestManager::g_lastBoastId = 0;
unsigned long CQuestManager::g_lastBoastValue = 0;
int CQuestManager::g_callCount = 0;

class CGameScriptInterface
{
public:
    virtual void SetBoastAsFailed(
        long boastId,
        const CCharString& boastName) const;
};

CQuestManager g_questManagerInstance;
CQuestManager* CGSISetBoastAsFailed_Manager = &g_questManagerInstance;

void CGameScriptInterface::SetBoastAsFailed(
    long boastId,
    const CCharString& boastName) const
{
    CQuestManager* const questManager =
        CGSISetBoastAsFailed_Manager;
    questManager->SetBoastAsCompleted(
        boastId,
        boastName);
}

int main()
{
    CGameScriptInterface iface;
    CCharString name(0xDEADBEEFu);

    iface.SetBoastAsFailed(42, name);

    if (CQuestManager::g_callCount == 1 &&
        CQuestManager::g_lastBoastId == 42 &&
        CQuestManager::g_lastBoastValue == 0xDEADBEEFu)
    {
        std::printf("TEST_PASS_SETBOASTASFAILED\n");
        return 0;
    }

    std::printf("TEST_FAIL id=%ld val=%lu count=%d\n",
        CQuestManager::g_lastBoastId,
        CQuestManager::g_lastBoastValue,
        CQuestManager::g_callCount);
    return 1;
}