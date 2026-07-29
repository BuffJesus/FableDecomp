#include <cstdio>

class CGSIHeroRenown_Player;
class CGSIHeroRenown_Node;

class CGSIHeroRenown_Manager
{
public:
    CGSIHeroRenown_Player* GetPlayer();
};

class CGSIHeroRenown_Player
{
public:
    void* GetHero();
};

class CGSIHeroRenown_Container
{
public:
    CGSIHeroRenown_Node* LowerBound(
        const long* pKey);

    CGSIHeroRenown_Node* m_pFirst;
    CGSIHeroRenown_Node* m_pEnd;
};

class CGSIHeroRenown_Component
{
public:
    void AddRenown(long renownPoints);
};

class CGSIHeroRenown_Node
{
public:
    long m_Key;
    CGSIHeroRenown_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual void GiveHeroRenownPoints(
        long renownPoints) const;
};

struct CGSIHeroRenown_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIHeroRenown_Manager* m_pManager;
};

struct CGSIHeroRenown_HeroStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIHeroRenown_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIHeroRenown_Player* g_pPlayer;
static CGSIHeroRenown_HeroStorage* g_pHero;
static CGSIHeroRenown_Node* g_pLowerBoundResult;
static CGSIHeroRenown_Container* g_pObservedContainer;
static CGSIHeroRenown_Component* g_pObservedComponent;
static long g_ObservedKey;
static long g_ObservedRenown;
static unsigned long g_LowerBoundCalls;
static unsigned long g_AddRenownCalls;

CGSIHeroRenown_Player*
CGSIHeroRenown_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIHeroRenown_Player::GetHero()
{
    return g_pHero;
}

CGSIHeroRenown_Node*
CGSIHeroRenown_Container::LowerBound(
    const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

void CGSIHeroRenown_Component::AddRenown(
    long renownPoints)
{
    g_pObservedComponent = this;
    g_ObservedRenown = renownPoints;
    ++g_AddRenownCalls;
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_pObservedComponent = 0;
    g_ObservedKey = -1;
    g_ObservedRenown = 0;
    g_LowerBoundCalls = 0;
    g_AddRenownCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_008990c0_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIHeroRenown_InterfaceStorage interfaceStorage = {};
    CGSIHeroRenown_Manager manager;
    CGSIHeroRenown_Player player;
    CGSIHeroRenown_HeroStorage hero = {};
    CGSIHeroRenown_Component fallbackComponent;
    CGSIHeroRenown_Component foundComponent;
    CGSIHeroRenown_Component endComponent;
    CGSIHeroRenown_Node foundNode = {};
    CGSIHeroRenown_Node endNode = {};

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
        GiveHeroRenownPoints(25);
    if (g_LowerBoundCalls != 0 ||
        g_AddRenownCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    pInterface->CGameScriptInterface::
        GiveHeroRenownPoints(25);
    if (g_AddRenownCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    const long fallbackArgument =
        reinterpret_cast<long>(&fallbackComponent);
    pInterface->CGameScriptInterface::
        GiveHeroRenownPoints(fallbackArgument);
    if (g_LowerBoundCalls != 0 ||
        g_AddRenownCalls != 1 ||
        g_pObservedComponent != &fallbackComponent ||
        g_ObservedRenown != fallbackArgument)
        return Fail("fallback component dispatch");

    ResetObservations();
    hero.m_Flags20 = 0x10;
    foundNode.m_Key = 4;
    g_pLowerBoundResult = &foundNode;
    pInterface->CGameScriptInterface::
        GiveHeroRenownPoints(75);
    if (g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 4 ||
        g_pObservedComponent != &foundComponent ||
        g_ObservedRenown != 75 ||
        g_LowerBoundCalls != 1 ||
        g_AddRenownCalls != 1)
        return Fail("found hero-stats component");

    ResetObservations();
    foundNode.m_Key = 5;
    g_pLowerBoundResult = &foundNode;
    pInterface->CGameScriptInterface::
        GiveHeroRenownPoints(125);
    if (g_pObservedComponent != &endComponent ||
        g_ObservedRenown != 125)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    pInterface->CGameScriptInterface::
        GiveHeroRenownPoints(175);
    if (g_pObservedComponent != &endComponent ||
        g_ObservedRenown != 175)
        return Fail("direct sentinel");

    std::printf("FSE2_008990c0_TEST PASS\n");
    return 0;
}
