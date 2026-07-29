#include <cstdio>

class CGSIWillEnergyGet_Player;
class CGSIWillEnergyGet_Node;

class CGSIWillEnergyGet_Manager
{
public:
    CGSIWillEnergyGet_Player* GetPlayer();
};

class CGSIWillEnergyGet_Player
{
public:
    void* GetHero();
};

class CGSIWillEnergyGet_Container
{
public:
    CGSIWillEnergyGet_Node* LowerBound(
        const long* pKey);

    CGSIWillEnergyGet_Node* m_pFirst;
    CGSIWillEnergyGet_Node* m_pEnd;
};

class CGSIWillEnergyGet_Component
{
public:
    unsigned char m_Pad00[0x58];
    long m_CurrentEnergy;
    long m_MaximumEnergy;
};

class CGSIWillEnergyGet_Node
{
public:
    long m_Key;
    CGSIWillEnergyGet_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual float GetHeroWillEnergyLevel() const;
};

struct CGSIWillEnergyGet_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIWillEnergyGet_Manager* m_pManager;
    unsigned char m_Pad18[0x40];
    long m_FallbackCurrent;
    long m_FallbackMaximum;
};

struct CGSIWillEnergyGet_HeroStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIWillEnergyGet_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIWillEnergyGet_Player* g_pPlayer;
static CGSIWillEnergyGet_HeroStorage* g_pHero;
static CGSIWillEnergyGet_Node* g_pLowerBoundResult;
static CGSIWillEnergyGet_Container* g_pObservedContainer;
static long g_ObservedKey;
static unsigned long g_LowerBoundCalls;

CGSIWillEnergyGet_Player*
CGSIWillEnergyGet_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIWillEnergyGet_Player::GetHero()
{
    return g_pHero;
}

CGSIWillEnergyGet_Node*
CGSIWillEnergyGet_Container::LowerBound(
    const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_ObservedKey = -1;
    g_LowerBoundCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_008999a0_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIWillEnergyGet_InterfaceStorage
        interfaceStorage = {};
    CGSIWillEnergyGet_Manager manager;
    CGSIWillEnergyGet_Player player;
    CGSIWillEnergyGet_HeroStorage hero = {};
    CGSIWillEnergyGet_Component foundComponent = {};
    CGSIWillEnergyGet_Component endComponent = {};
    CGSIWillEnergyGet_Node foundNode = {};
    CGSIWillEnergyGet_Node endNode = {};

    interfaceStorage.m_pManager = &manager;
    interfaceStorage.m_FallbackCurrent = 25;
    interfaceStorage.m_FallbackMaximum = 100;
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
            GetHeroWillEnergyLevel() != 0.0f ||
        g_LowerBoundCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            GetHeroWillEnergyLevel() != 0.0f ||
        g_LowerBoundCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroWillEnergyLevel() != 0.25f ||
        g_LowerBoundCalls != 0)
        return Fail("fallback energy ratio");

    ResetObservations();
    hero.m_Flags20 = 0x10;
    foundNode.m_Key = 4;
    foundComponent.m_CurrentEnergy = 3;
    foundComponent.m_MaximumEnergy = 4;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroWillEnergyLevel() != 0.75f ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 4 ||
        g_LowerBoundCalls != 1)
        return Fail("found HeroStats energy ratio");

    ResetObservations();
    foundNode.m_Key = 5;
    endComponent.m_CurrentEnergy = 1;
    endComponent.m_MaximumEnergy = 2;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroWillEnergyLevel() != 0.5f)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (pInterface->CGameScriptInterface::
            GetHeroWillEnergyLevel() != 0.5f)
        return Fail("direct sentinel");

    std::printf("FSE2_008999a0_TEST PASS\n");
    return 0;
}
