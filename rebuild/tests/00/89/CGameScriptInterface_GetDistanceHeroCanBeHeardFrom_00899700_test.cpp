#include <cstdio>

class CGSIHeroHearing_Player;
class CGSIHeroHearing_Node;

class CGSIHeroHearing_Manager
{
public:
    CGSIHeroHearing_Player* GetPlayer();
};

class CGSIHeroHearing_Player
{
public:
    void* GetHero();
};

class CGSIHeroHearing_Container
{
public:
    CGSIHeroHearing_Node* LowerBound(
        const long* pKey);

    CGSIHeroHearing_Node* m_pFirst;
    CGSIHeroHearing_Node* m_pEnd;
};

class CGSIHeroHearing_Component
{
public:
    unsigned char m_Pad00[0x14];
    float m_HearingDistance;
};

class CGSIHeroHearing_Node
{
public:
    long m_Key;
    CGSIHeroHearing_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual float GetDistanceHeroCanBeHeardFrom() const;
};

struct CGSIHeroHearing_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIHeroHearing_Manager* m_pManager;
};

struct CGSIHeroHearing_HeroStorage
{
    unsigned char m_Pad00[0x24];
    unsigned long m_Flags24;
    unsigned char m_Pad28[0x1C];
    CGSIHeroHearing_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

union CGSIHeroHearing_FloatBits
{
    float m_Float;
    unsigned long m_Bits;
};

static CGSIHeroHearing_Player* g_pPlayer;
static CGSIHeroHearing_HeroStorage* g_pHero;
static CGSIHeroHearing_Node* g_pLowerBoundResult;
static CGSIHeroHearing_Container* g_pObservedContainer;
static long g_ObservedKey;
static unsigned long g_LowerBoundCalls;

CGSIHeroHearing_Player*
CGSIHeroHearing_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIHeroHearing_Player::GetHero()
{
    return g_pHero;
}

CGSIHeroHearing_Node*
CGSIHeroHearing_Container::LowerBound(
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
        "FSE2_00899700_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIHeroHearing_InterfaceStorage
        interfaceStorage = {};
    CGSIHeroHearing_Manager manager;
    CGSIHeroHearing_Player player;
    CGSIHeroHearing_HeroStorage hero = {};
    CGSIHeroHearing_Component foundComponent = {};
    CGSIHeroHearing_Component endComponent = {};
    CGSIHeroHearing_Node foundNode = {};
    CGSIHeroHearing_Node endNode = {};

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
            GetDistanceHeroCanBeHeardFrom() != 0.0f ||
        g_LowerBoundCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            GetDistanceHeroCanBeHeardFrom() != 0.0f ||
        g_LowerBoundCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags24 = 0;
    CGSIHeroHearing_FloatBits expectedFallback;
    expectedFallback.m_Bits =
        reinterpret_cast<unsigned long>(&manager);
    CGSIHeroHearing_FloatBits actualFallback;
    actualFallback.m_Float =
        pInterface->CGameScriptInterface::
            GetDistanceHeroCanBeHeardFrom();
    if (actualFallback.m_Bits !=
            expectedFallback.m_Bits ||
        g_LowerBoundCalls != 0)
        return Fail("saved-interface raw fallback");

    ResetObservations();
    hero.m_Flags24 = 0x00400000;
    foundNode.m_Key = 0x36;
    foundComponent.m_HearingDistance = 24.5f;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetDistanceHeroCanBeHeardFrom() != 24.5f ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 0x36 ||
        g_LowerBoundCalls != 1)
        return Fail("found hearing component");

    ResetObservations();
    foundNode.m_Key = 0x37;
    endComponent.m_HearingDistance = 48.25f;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetDistanceHeroCanBeHeardFrom() != 48.25f)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (pInterface->CGameScriptInterface::
            GetDistanceHeroCanBeHeardFrom() != 48.25f)
        return Fail("direct sentinel");

    std::printf("FSE2_00899700_TEST PASS\n");
    return 0;
}
