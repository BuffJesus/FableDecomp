#include <cstdio>

class CGSIHeroFatness_Player;
class CGSIHeroFatness_Node;

class CGSIHeroFatness_Manager
{
public:
    CGSIHeroFatness_Player* GetPlayer();
};

class CGSIHeroFatness_Player
{
public:
    void* GetHero();
};

class CGSIHeroFatness_Container
{
public:
    CGSIHeroFatness_Node* LowerBound(
        const long* pKey);

    CGSIHeroFatness_Node* m_pFirst;
    CGSIHeroFatness_Node* m_pEnd;
};

class CGSIHeroFatness_Component
{
public:
    unsigned char m_Pad00[0x38];
    float m_Fatness;
};

class CGSIHeroFatness_Node
{
public:
    long m_Key;
    CGSIHeroFatness_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual float GetHeroFatness() const;
};

struct CGSIHeroFatness_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIHeroFatness_Manager* m_pManager;
    unsigned char m_Pad18[0x20];
    float m_FallbackFatness;
};

struct CGSIHeroFatness_HeroStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIHeroFatness_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIHeroFatness_Player* g_pPlayer;
static CGSIHeroFatness_HeroStorage* g_pHero;
static CGSIHeroFatness_Node* g_pLowerBoundResult;
static CGSIHeroFatness_Container* g_pObservedContainer;
static long g_ObservedKey;
static unsigned long g_LowerBoundCalls;

CGSIHeroFatness_Player*
CGSIHeroFatness_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIHeroFatness_Player::GetHero()
{
    return g_pHero;
}

CGSIHeroFatness_Node*
CGSIHeroFatness_Container::LowerBound(
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
        "FSE2_00899850_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIHeroFatness_InterfaceStorage interfaceStorage = {};
    CGSIHeroFatness_Manager manager;
    CGSIHeroFatness_Player player;
    CGSIHeroFatness_HeroStorage hero = {};
    CGSIHeroFatness_Component foundComponent = {};
    CGSIHeroFatness_Component endComponent = {};
    CGSIHeroFatness_Node foundNode = {};
    CGSIHeroFatness_Node endNode = {};

    interfaceStorage.m_pManager = &manager;
    interfaceStorage.m_FallbackFatness = 12.5f;
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
            GetHeroFatness() != -999.0f ||
        g_LowerBoundCalls != 0)
        return Fail("null hero sentinel");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            GetHeroFatness() != -999.0f ||
        g_LowerBoundCalls != 0)
        return Fail("dead hero sentinel");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroFatness() != 12.5f ||
        g_LowerBoundCalls != 0)
        return Fail("fallback interface value");

    ResetObservations();
    hero.m_Flags20 = 0x10;
    foundNode.m_Key = 4;
    foundComponent.m_Fatness = 34.75f;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroFatness() != 34.75f ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 4 ||
        g_LowerBoundCalls != 1)
        return Fail("found HeroStats fatness");

    ResetObservations();
    foundNode.m_Key = 5;
    endComponent.m_Fatness = 56.25f;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroFatness() != 56.25f)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (pInterface->CGameScriptInterface::
            GetHeroFatness() != 56.25f)
        return Fail("direct sentinel");

    std::printf("FSE2_00899850_TEST PASS\n");
    return 0;
}
