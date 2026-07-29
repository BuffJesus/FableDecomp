#include <cstdio>

class CGSIHeroScariness_Player;
class CGSIHeroScariness_Node;

class CGSIHeroScariness_Manager
{
public:
    CGSIHeroScariness_Player* GetPlayer();
};

class CGSIHeroScariness_Player
{
public:
    void* GetHero();
};

class CGSIHeroScariness_Container
{
public:
    CGSIHeroScariness_Node* LowerBound(
        const long* pKey);

    CGSIHeroScariness_Node* m_pFirst;
    CGSIHeroScariness_Node* m_pEnd;
};

class CGSIHeroScariness_Component
{
public:
    unsigned char m_Pad00[0xA0];
    float m_Scariness;
};

class CGSIHeroScariness_Node
{
public:
    long m_Key;
    CGSIHeroScariness_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual float GetHeroScariness() const;
};

struct CGSIHeroScariness_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIHeroScariness_Manager* m_pManager;
    unsigned char m_Pad18[0x88];
    float m_FallbackScariness;
};

struct CGSIHeroScariness_HeroStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIHeroScariness_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIHeroScariness_Player* g_pPlayer;
static CGSIHeroScariness_HeroStorage* g_pHero;
static CGSIHeroScariness_Node* g_pLowerBoundResult;
static CGSIHeroScariness_Container* g_pObservedContainer;
static long g_ObservedKey;
static unsigned long g_LowerBoundCalls;

CGSIHeroScariness_Player*
CGSIHeroScariness_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIHeroScariness_Player::GetHero()
{
    return g_pHero;
}

CGSIHeroScariness_Node*
CGSIHeroScariness_Container::LowerBound(
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
        "FSE2_008998c0_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIHeroScariness_InterfaceStorage
        interfaceStorage = {};
    CGSIHeroScariness_Manager manager;
    CGSIHeroScariness_Player player;
    CGSIHeroScariness_HeroStorage hero = {};
    CGSIHeroScariness_Component foundComponent = {};
    CGSIHeroScariness_Component endComponent = {};
    CGSIHeroScariness_Node foundNode = {};
    CGSIHeroScariness_Node endNode = {};

    interfaceStorage.m_pManager = &manager;
    interfaceStorage.m_FallbackScariness = 14.5f;
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
            GetHeroScariness() != 0.0f ||
        g_LowerBoundCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            GetHeroScariness() != 0.0f ||
        g_LowerBoundCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroScariness() != 14.5f ||
        g_LowerBoundCalls != 0)
        return Fail("fallback interface value");

    ResetObservations();
    hero.m_Flags20 = 0x10;
    foundNode.m_Key = 4;
    foundComponent.m_Scariness = 36.75f;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroScariness() != 36.75f ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 4 ||
        g_LowerBoundCalls != 1)
        return Fail("found HeroStats scariness");

    ResetObservations();
    foundNode.m_Key = 5;
    endComponent.m_Scariness = 58.25f;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroScariness() != 58.25f)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (pInterface->CGameScriptInterface::
            GetHeroScariness() != 58.25f)
        return Fail("direct sentinel");

    std::printf("FSE2_008998c0_TEST PASS\n");
    return 0;
}
