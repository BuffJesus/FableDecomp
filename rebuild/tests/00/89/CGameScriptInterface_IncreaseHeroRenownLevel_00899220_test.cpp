#include <cstdio>

class CGSIIncreaseRenown_Player;
class CGSIIncreaseRenown_Node;

class CGSIIncreaseRenown_Manager
{
public:
    CGSIIncreaseRenown_Player* GetPlayer();
};

class CGSIIncreaseRenown_Player
{
public:
    void* GetHero();
};

class CGSIIncreaseRenown_Container
{
public:
    CGSIIncreaseRenown_Node* LowerBound(
        const long* pKey);

    CGSIIncreaseRenown_Node* m_pFirst;
    CGSIIncreaseRenown_Node* m_pEnd;
};

class CGSIIncreaseRenown_Component
{
public:
    void UpgradeRenownLevel();
};

class CGSIIncreaseRenown_Node
{
public:
    long m_Key;
    CGSIIncreaseRenown_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual void IncreaseHeroRenownLevel() const;
};

struct CGSIIncreaseRenown_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIIncreaseRenown_Manager* m_pManager;
};

struct CGSIIncreaseRenown_HeroStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIIncreaseRenown_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIIncreaseRenown_Player* g_pPlayer;
static CGSIIncreaseRenown_HeroStorage* g_pHero;
static CGSIIncreaseRenown_Node* g_pLowerBoundResult;
static CGSIIncreaseRenown_Container* g_pObservedContainer;
static CGSIIncreaseRenown_Component* g_pObservedComponent;
static long g_ObservedKey;
static unsigned long g_LowerBoundCalls;
static unsigned long g_UpgradeCalls;

CGSIIncreaseRenown_Player*
CGSIIncreaseRenown_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIIncreaseRenown_Player::GetHero()
{
    return g_pHero;
}

CGSIIncreaseRenown_Node*
CGSIIncreaseRenown_Container::LowerBound(
    const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

void CGSIIncreaseRenown_Component::UpgradeRenownLevel()
{
    g_pObservedComponent = this;
    ++g_UpgradeCalls;
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_pObservedComponent = 0;
    g_ObservedKey = -1;
    g_LowerBoundCalls = 0;
    g_UpgradeCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_00899220_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIIncreaseRenown_InterfaceStorage
        interfaceStorage = {};
    CGSIIncreaseRenown_Manager manager;
    CGSIIncreaseRenown_Player player;
    CGSIIncreaseRenown_HeroStorage hero = {};
    CGSIIncreaseRenown_Component foundComponent;
    CGSIIncreaseRenown_Component endComponent;
    CGSIIncreaseRenown_Node foundNode = {};
    CGSIIncreaseRenown_Node endNode = {};

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
        IncreaseHeroRenownLevel();
    if (g_UpgradeCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    pInterface->CGameScriptInterface::
        IncreaseHeroRenownLevel();
    if (g_UpgradeCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    pInterface->CGameScriptInterface::
        IncreaseHeroRenownLevel();
    if (g_LowerBoundCalls != 0 ||
        g_UpgradeCalls != 1 ||
        g_pObservedComponent !=
            reinterpret_cast<
                CGSIIncreaseRenown_Component*>(
                    &interfaceStorage))
        return Fail("fallback interface dispatch");

    ResetObservations();
    hero.m_Flags20 = 0x10;
    foundNode.m_Key = 4;
    g_pLowerBoundResult = &foundNode;
    pInterface->CGameScriptInterface::
        IncreaseHeroRenownLevel();
    if (g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 4 ||
        g_pObservedComponent != &foundComponent ||
        g_LowerBoundCalls != 1 ||
        g_UpgradeCalls != 1)
        return Fail("found HeroStats dispatch");

    ResetObservations();
    foundNode.m_Key = 5;
    g_pLowerBoundResult = &foundNode;
    pInterface->CGameScriptInterface::
        IncreaseHeroRenownLevel();
    if (g_pObservedComponent != &endComponent)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    pInterface->CGameScriptInterface::
        IncreaseHeroRenownLevel();
    if (g_pObservedComponent != &endComponent)
        return Fail("direct sentinel");

    std::printf("FSE2_00899220_TEST PASS\n");
    return 0;
}
