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
    void AddFeatCard(
        long featCardId,
        const CCharString& title,
        const CCharString& description);
};

class CGameScriptInterface
{
public:
    virtual void AddFeatCard(
        long featCardId,
        const CCharString& title,
        const CCharString& description) const;
};

CQuestManager* CGSIAddFeatCard_Manager;
static CQuestManager* g_ObservedManager;
static long g_ObservedFeatCardId;
static const CCharString* g_ObservedTitle;
static const CCharString* g_ObservedDescription;
static unsigned long g_CallCount;

void CQuestManager::AddFeatCard(
    long featCardId,
    const CCharString& title,
    const CCharString& description)
{
    g_ObservedManager = this;
    g_ObservedFeatCardId = featCardId;
    g_ObservedTitle = &title;
    g_ObservedDescription = &description;
    ++g_CallCount;
}

int main()
{
    CQuestManager manager;
    CGameScriptInterface interfaceObject;
    CCharString firstTitle(0x11223344);
    CCharString firstDescription(0x55667788);
    CCharString secondTitle(0x99aabbcc);
    CCharString secondDescription(0xddeeff00);
    CGSIAddFeatCard_Manager = &manager;

    interfaceObject.AddFeatCard(
        -1234567,
        firstTitle,
        firstDescription);
    if (g_CallCount != 1 || g_ObservedManager != &manager ||
        g_ObservedFeatCardId != -1234567 ||
        g_ObservedTitle != &firstTitle ||
        g_ObservedDescription != &firstDescription)
        return 1;

    interfaceObject.AddFeatCard(
        0x76543210,
        secondTitle,
        secondDescription);
    if (g_CallCount != 2 || g_ObservedManager != &manager ||
        g_ObservedFeatCardId != 0x76543210 ||
        g_ObservedTitle != &secondTitle ||
        g_ObservedDescription != &secondDescription)
        return 2;

    std::printf("FSE2_00891540_TEST PASS\n");
    return 0;
}
