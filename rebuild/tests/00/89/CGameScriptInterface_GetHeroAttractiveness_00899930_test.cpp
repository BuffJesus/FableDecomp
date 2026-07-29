#include <cstdio>

class CGSIHeroAttractiveness_Player;
class CGSIHeroAttractiveness_Node;

class CGSIHeroAttractiveness_Manager
{
public:
    CGSIHeroAttractiveness_Player* GetPlayer();
};

class CGSIHeroAttractiveness_Player
{
public:
    void* GetHero();
};

class CGSIHeroAttractiveness_Container
{
public:
    CGSIHeroAttractiveness_Node* LowerBound(
        const long* pKey);

    CGSIHeroAttractiveness_Node* m_pFirst;
    CGSIHeroAttractiveness_Node* m_pEnd;
};

class CGSIHeroAttractiveness_Component
{
public:
    unsigned char m_Pad00[0x9C];
    float m_Attractiveness;
};

class CGSIHeroAttractiveness_Node
{
public:
    long m_Key;
    CGSIHeroAttractiveness_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual float GetHeroAttractiveness() const;
};

struct CGSIHeroAttractiveness_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIHeroAttractiveness_Manager* m_pManager;
    unsigned char m_Pad18[0x84];
    float m_FallbackAttractiveness;
};

struct CGSIHeroAttractiveness_HeroStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIHeroAttractiveness_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIHeroAttractiveness_Player* g_pPlayer;
static CGSIHeroAttractiveness_HeroStorage* g_pHero;
static CGSIHeroAttractiveness_Node* g_pLowerBoundResult;
static CGSIHeroAttractiveness_Container* g_pObservedContainer;
static long g_ObservedKey;
static unsigned long g_LowerBoundCalls;

CGSIHeroAttractiveness_Player*
CGSIHeroAttractiveness_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIHeroAttractiveness_Player::GetHero()
{
    return g_pHero;
}

CGSIHeroAttractiveness_Node*
CGSIHeroAttractiveness_Container::LowerBound(
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
        "FSE2_00899930_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIHeroAttractiveness_InterfaceStorage
        interfaceStorage = {};
    CGSIHeroAttractiveness_Manager manager;
    CGSIHeroAttractiveness_Player player;
    CGSIHeroAttractiveness_HeroStorage hero = {};
    CGSIHeroAttractiveness_Component foundComponent = {};
    CGSIHeroAttractiveness_Component endComponent = {};
    CGSIHeroAttractiveness_Node foundNode = {};
    CGSIHeroAttractiveness_Node endNode = {};

    interfaceStorage.m_pManager = &manager;
    interfaceStorage.m_FallbackAttractiveness = 16.5f;
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
            GetHeroAttractiveness() != 0.0f ||
        g_LowerBoundCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            GetHeroAttractiveness() != 0.0f ||
        g_LowerBoundCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroAttractiveness() != 16.5f ||
        g_LowerBoundCalls != 0)
        return Fail("fallback interface value");

    ResetObservations();
    hero.m_Flags20 = 0x10;
    foundNode.m_Key = 4;
    foundComponent.m_Attractiveness = 38.75f;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroAttractiveness() != 38.75f ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 4 ||
        g_LowerBoundCalls != 1)
        return Fail("found HeroStats attractiveness");

    ResetObservations();
    foundNode.m_Key = 5;
    endComponent.m_Attractiveness = 60.25f;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroAttractiveness() != 60.25f)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (pInterface->CGameScriptInterface::
            GetHeroAttractiveness() != 60.25f)
        return Fail("direct sentinel");

    std::printf("FSE2_00899930_TEST PASS\n");
    return 0;
}
