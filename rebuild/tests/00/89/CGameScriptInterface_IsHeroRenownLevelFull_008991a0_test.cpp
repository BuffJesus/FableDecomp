#include <cstdio>

class CGSIRenownFull_Player;
class CGSIRenownFull_Node;

class CGSIRenownFull_Manager
{
public:
    CGSIRenownFull_Player* GetPlayer();
};

class CGSIRenownFull_Player
{
public:
    void* GetHero();
};

class CGSIRenownFull_Container
{
public:
    CGSIRenownFull_Node* LowerBound(
        const long* pKey);

    CGSIRenownFull_Node* m_pFirst;
    CGSIRenownFull_Node* m_pEnd;
};

class CGSIRenownFull_Component
{
public:
    float GetPercentageThroughRenownLevel();

    float m_Percentage;
};

class CGSIRenownFull_Node
{
public:
    long m_Key;
    CGSIRenownFull_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual bool IsHeroRenownLevelFull() const;
};

struct CGSIRenownFull_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIRenownFull_Manager* m_pManager;
};

struct CGSIRenownFull_HeroStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIRenownFull_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIRenownFull_Player* g_pPlayer;
static CGSIRenownFull_HeroStorage* g_pHero;
static CGSIRenownFull_Node* g_pLowerBoundResult;
static void* g_pFallbackComponent;
static float g_FallbackPercentage;
static CGSIRenownFull_Container* g_pObservedContainer;
static CGSIRenownFull_Component* g_pObservedComponent;
static long g_ObservedKey;
static unsigned long g_LowerBoundCalls;
static unsigned long g_PercentageCalls;

CGSIRenownFull_Player*
CGSIRenownFull_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIRenownFull_Player::GetHero()
{
    return g_pHero;
}

CGSIRenownFull_Node*
CGSIRenownFull_Container::LowerBound(
    const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

float CGSIRenownFull_Component::
GetPercentageThroughRenownLevel()
{
    g_pObservedComponent = this;
    ++g_PercentageCalls;
    if (this == g_pFallbackComponent)
        return g_FallbackPercentage;
    return m_Percentage;
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_pObservedComponent = 0;
    g_ObservedKey = -1;
    g_LowerBoundCalls = 0;
    g_PercentageCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_008991a0_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIRenownFull_InterfaceStorage interfaceStorage = {};
    CGSIRenownFull_Manager manager;
    CGSIRenownFull_Player player;
    CGSIRenownFull_HeroStorage hero = {};
    CGSIRenownFull_Component foundComponent = {};
    CGSIRenownFull_Component endComponent = {};
    CGSIRenownFull_Node foundNode = {};
    CGSIRenownFull_Node endNode = {};

    interfaceStorage.m_pManager = &manager;
    CGameScriptInterface* const pInterface =
        reinterpret_cast<CGameScriptInterface*>(
            &interfaceStorage);
    g_pFallbackComponent = &interfaceStorage;
    g_pPlayer = &player;
    foundNode.m_pComponent = &foundComponent;
    endNode.m_pComponent = &endComponent;
    hero.m_Container.m_pEnd = &endNode;

    ResetObservations();
    g_pHero = 0;
    if (pInterface->CGameScriptInterface::
            IsHeroRenownLevelFull() ||
        g_PercentageCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            IsHeroRenownLevelFull() ||
        g_PercentageCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    g_FallbackPercentage = 0.9998f;
    if (pInterface->CGameScriptInterface::
            IsHeroRenownLevelFull() ||
        g_pObservedComponent !=
            reinterpret_cast<CGSIRenownFull_Component*>(
                &interfaceStorage) ||
        g_PercentageCalls != 1)
        return Fail("fallback below threshold");

    ResetObservations();
    g_FallbackPercentage = 1.0f;
    if (!pInterface->CGameScriptInterface::
            IsHeroRenownLevelFull())
        return Fail("fallback above threshold");

    ResetObservations();
    hero.m_Flags20 = 0x10;
    foundNode.m_Key = 4;
    foundComponent.m_Percentage = 0.9998f;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            IsHeroRenownLevelFull() ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 4 ||
        g_pObservedComponent != &foundComponent)
        return Fail("found component below threshold");

    ResetObservations();
    foundComponent.m_Percentage = 1.0f;
    if (!pInterface->CGameScriptInterface::
            IsHeroRenownLevelFull())
        return Fail("found component above threshold");

    ResetObservations();
    foundNode.m_Key = 5;
    endComponent.m_Percentage = 1.0f;
    g_pLowerBoundResult = &foundNode;
    if (!pInterface->CGameScriptInterface::
            IsHeroRenownLevelFull() ||
        g_pObservedComponent != &endComponent)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (!pInterface->CGameScriptInterface::
            IsHeroRenownLevelFull() ||
        g_pObservedComponent != &endComponent)
        return Fail("direct sentinel");

    std::printf("FSE2_008991a0_TEST PASS\n");
    return 0;
}
