#include <cstdio>

class CGSIWillRefill_Player;
class CGSIWillRefill_Node;

class CGSIWillRefill_Manager
{
public:
    CGSIWillRefill_Player* GetPlayer();
};

class CGSIWillRefill_Player
{
public:
    void* GetHero();
};

class CGSIWillRefill_Container
{
public:
    CGSIWillRefill_Node* LowerBound(
        const long* pKey);

    CGSIWillRefill_Node* m_pFirst;
    CGSIWillRefill_Node* m_pEnd;
};

class CGSIWillRefill_Component
{
public:
    unsigned char m_Pad00[0x68];
    unsigned char m_RefillDisabled;
};

class CGSIWillRefill_Node
{
public:
    long m_Key;
    CGSIWillRefill_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual void SetHeroWillEnergyAsAbleToRefill(
        bool ableToRefill) const;
};

struct CGSIWillRefill_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIWillRefill_Manager* m_pManager;
};

struct CGSIWillRefill_HeroStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIWillRefill_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIWillRefill_Player* g_pPlayer;
static CGSIWillRefill_HeroStorage* g_pHero;
static CGSIWillRefill_Node* g_pLowerBoundResult;
static CGSIWillRefill_Container* g_pObservedContainer;
static long g_ObservedKey;
static unsigned long g_LowerBoundCalls;

CGSIWillRefill_Player*
CGSIWillRefill_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIWillRefill_Player::GetHero()
{
    return g_pHero;
}

CGSIWillRefill_Node*
CGSIWillRefill_Container::LowerBound(
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
        "FSE2_00899a90_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIWillRefill_InterfaceStorage interfaceStorage = {};
    CGSIWillRefill_Manager manager;
    CGSIWillRefill_Player player;
    CGSIWillRefill_HeroStorage hero = {};
    CGSIWillRefill_Component foundComponent = {};
    CGSIWillRefill_Component endComponent = {};
    CGSIWillRefill_Node foundNode = {};
    CGSIWillRefill_Node endNode = {};

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
    pInterface->CGameScriptInterface::
        SetHeroWillEnergyAsAbleToRefill(true);
    if (g_LowerBoundCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    pInterface->CGameScriptInterface::
        SetHeroWillEnergyAsAbleToRefill(true);
    if (g_LowerBoundCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0x10;
    foundNode.m_Key = 4;
    foundComponent.m_RefillDisabled = 1;
    g_pLowerBoundResult = &foundNode;
    pInterface->CGameScriptInterface::
        SetHeroWillEnergyAsAbleToRefill(true);
    if (g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 4 ||
        g_LowerBoundCalls != 1 ||
        foundComponent.m_RefillDisabled != 0)
        return Fail("found component enable refill");

    ResetObservations();
    foundComponent.m_RefillDisabled = 0;
    pInterface->CGameScriptInterface::
        SetHeroWillEnergyAsAbleToRefill(false);
    if (foundComponent.m_RefillDisabled != 1)
        return Fail("found component disable refill");

    ResetObservations();
    foundNode.m_Key = 5;
    endComponent.m_RefillDisabled = 1;
    g_pLowerBoundResult = &foundNode;
    pInterface->CGameScriptInterface::
        SetHeroWillEnergyAsAbleToRefill(true);
    if (endComponent.m_RefillDisabled != 0)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    pInterface->CGameScriptInterface::
        SetHeroWillEnergyAsAbleToRefill(false);
    if (endComponent.m_RefillDisabled != 1)
        return Fail("direct sentinel");

    std::printf("FSE2_00899a90_TEST PASS\n");
    return 0;
}
