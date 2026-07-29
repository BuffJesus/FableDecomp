#include <cstdio>

class CGSIExperience_Player;
class CGSIExperience_Node;

class CGSIExperience_Manager
{
public:
    CGSIExperience_Player* GetPlayer();
};

class CGSIExperience_Player
{
public:
    void* GetHero();
};

class CGSIExperience_Container
{
public:
    CGSIExperience_Node* LowerBound(const long* pKey);

    CGSIExperience_Node* m_pFirst;
    CGSIExperience_Node* m_pEnd;
};

class CGSIExperience_Component
{
public:
    unsigned char m_Pad00[0x14];
    long m_AvailableExperience;
};

class CGSIExperience_Node
{
public:
    long m_Key;
    CGSIExperience_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual long GetHeroExperienceAvailableToSpend() const;
};

struct InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIExperience_Manager* m_pManager;
};

struct EntityStorage
{
    unsigned char m_Pad00[0x2C];
    unsigned long m_Flags2C;
    unsigned char m_Pad30[0x14];
    CGSIExperience_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIExperience_Player* g_pPlayer;
static EntityStorage* g_pHero;
static CGSIExperience_Node* g_pLowerBoundResult;
static CGSIExperience_Manager* g_pObservedManager;
static CGSIExperience_Player* g_pObservedPlayer;
static CGSIExperience_Container* g_pObservedContainer;
static long g_ObservedKey;
static unsigned long g_ManagerCalls;
static unsigned long g_PlayerCalls;
static unsigned long g_LowerBoundCalls;

CGSIExperience_Player* CGSIExperience_Manager::GetPlayer()
{
    g_pObservedManager = this;
    ++g_ManagerCalls;
    return g_pPlayer;
}

void* CGSIExperience_Player::GetHero()
{
    g_pObservedPlayer = this;
    ++g_PlayerCalls;
    return g_pHero;
}

CGSIExperience_Node*
CGSIExperience_Container::LowerBound(const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

static void ResetObservations()
{
    g_pObservedManager = 0;
    g_pObservedPlayer = 0;
    g_pObservedContainer = 0;
    g_ObservedKey = 0;
    g_ManagerCalls = 0;
    g_PlayerCalls = 0;
    g_LowerBoundCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_008997e0_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    InterfaceStorage interfaceStorage = {};
    CGSIExperience_Manager manager;
    CGSIExperience_Player player;
    EntityStorage hero = {};
    CGSIExperience_Component foundComponent = {};
    CGSIExperience_Component endComponent = {};
    CGSIExperience_Node foundNode = {};
    CGSIExperience_Node endNode = {};

    interfaceStorage.m_pManager = &manager;
    CGameScriptInterface* const pInterface =
        reinterpret_cast<CGameScriptInterface*>(
            &interfaceStorage);
    g_pPlayer = &player;
    foundNode.m_pComponent = &foundComponent;
    endNode.m_pComponent = &endComponent;
    hero.m_Container.m_pEnd = &endNode;

    ResetObservations();
    g_pHero = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroExperienceAvailableToSpend() != 0 ||
        g_ManagerCalls != 1 || g_PlayerCalls != 1 ||
        g_LowerBoundCalls != 0)
        return Fail("null hero guard");

    ResetObservations();
    g_pHero = &hero;
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            GetHeroExperienceAvailableToSpend() != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags2C = 0;
    const long fallbackValue =
        pInterface->CGameScriptInterface::
            GetHeroExperienceAvailableToSpend();
    if (fallbackValue !=
            *reinterpret_cast<long*>(
                reinterpret_cast<unsigned char*>(
                    &interfaceStorage) + 0x14) ||
        g_LowerBoundCalls != 0)
        return Fail("interface fallback value");

    ResetObservations();
    hero.m_Flags2C = 0x100;
    foundNode.m_Key = 0x68;
    foundComponent.m_AvailableExperience = 1234567;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroExperienceAvailableToSpend() != 1234567 ||
        g_pObservedManager != &manager ||
        g_pObservedPlayer != &player ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 0x68)
        return Fail("found component value");

    ResetObservations();
    foundNode.m_pComponent = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroExperienceAvailableToSpend() != 0)
        return Fail("null component");
    foundNode.m_pComponent = &foundComponent;

    ResetObservations();
    foundNode.m_Key = 0x69;
    endComponent.m_AvailableExperience = -7654321;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroExperienceAvailableToSpend() != -7654321)
        return Fail("greater-key sentinel value");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (pInterface->CGameScriptInterface::
            GetHeroExperienceAvailableToSpend() != -7654321)
        return Fail("direct sentinel value");

    std::printf("FSE2_008997e0_TEST PASS\n");
    return 0;
}
