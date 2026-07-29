#include <cstdio>

class CCharString
{
public:
    explicit CCharString(unsigned long value)
        : m_Value(value)
    {
    }

    CCharString(const CCharString& other);

    unsigned long m_Value;
};

class CQuestManager
{
public:
    void SetBoastAsCompleted(
        long boastId,
        CCharString boastName);
};

class CGameScriptInterface
{
public:
    virtual void SetBoastAsCompleted(
        long boastId,
        const CCharString& boastName) const;
};

CQuestManager* CGSISetBoastAsCompleted_Manager;
static CQuestManager* g_ObservedManager;
static const CCharString* g_CopySource;
static CCharString* g_CopyDestination;
static unsigned long g_ObservedNameValue;
static long g_ObservedBoastId;
static unsigned long g_CopyCount;
static unsigned long g_CallCount;

CCharString::CCharString(const CCharString& other)
    : m_Value(other.m_Value)
{
    g_CopySource = &other;
    g_CopyDestination = this;
    ++g_CopyCount;
}

void CQuestManager::SetBoastAsCompleted(
    long boastId,
    CCharString boastName)
{
    g_ObservedManager = this;
    g_ObservedBoastId = boastId;
    g_ObservedNameValue = boastName.m_Value;
    if (&boastName != g_CopyDestination)
        g_CopyCount = 0xFFFFFFFFUL;
    ++g_CallCount;
}

static int CheckCall(
    CGameScriptInterface& interfaceObject,
    CQuestManager& manager,
    long boastId,
    const CCharString& boastName)
{
    g_ObservedManager = 0;
    g_CopySource = 0;
    g_CopyDestination = 0;
    g_ObservedNameValue = 0;
    g_ObservedBoastId = 0;
    g_CopyCount = 0;
    g_CallCount = 0;
    CGSISetBoastAsCompleted_Manager = &manager;

    interfaceObject.SetBoastAsCompleted(boastId, boastName);

    if (g_CallCount != 1 || g_ObservedManager != &manager)
        return 1;
    if (g_CopyCount != 1 || g_CopySource != &boastName)
        return 2;
    if (g_ObservedBoastId != boastId ||
        g_ObservedNameValue != boastName.m_Value)
        return 3;
    return 0;
}

int main()
{
    CQuestManager manager;
    CGameScriptInterface interfaceObject;
    CCharString firstBoast(0x11223344);
    CCharString secondBoast(0x55667788);

    int result = CheckCall(interfaceObject, manager, 0, firstBoast);
    if (result != 0)
        return result;

    result = CheckCall(
        interfaceObject,
        manager,
        static_cast<long>(0x87654321UL),
        secondBoast);
    if (result != 0)
        return result + 3;

    std::printf("FSE2_008915e0_TEST PASS\n");
    return 0;
}
