#include <cstdio>

class CGSIHeroAgeGet_Player;
class CGSIHeroAgeGet_Node;

class CGSIHeroAgeGet_Manager
{
public:
    CGSIHeroAgeGet_Player* GetPlayer();
};

class CGSIHeroAgeGet_Player
{
public:
    void* GetHero();
};

class CGSIHeroAgeGet_Container
{
public:
    CGSIHeroAgeGet_Node* LowerBound(
        const long* pKey);

    CGSIHeroAgeGet_Node* m_pFirst;
    CGSIHeroAgeGet_Node* m_pEnd;
};

class CGSIHeroAgeGet_Component
{
public:
    unsigned char m_Pad00[0x30];
    float m_Age;
};

class CGSIHeroAgeGet_Node
{
public:
    long m_Key;
    CGSIHeroAgeGet_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual float GetHeroAge() const;
};

struct CGSIHeroAgeGet_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIHeroAgeGet_Manager* m_pManager;
    unsigned char m_Pad18[0x18];
    float m_FallbackAge;
};

struct CGSIHeroAgeGet_HeroStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIHeroAgeGet_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIHeroAgeGet_Player* g_pPlayer;
static CGSIHeroAgeGet_HeroStorage* g_pHero;
static CGSIHeroAgeGet_Node* g_pLowerBoundResult;
static CGSIHeroAgeGet_Container* g_pObservedContainer;
static long g_ObservedKey;
static unsigned long g_LowerBoundCalls;

CGSIHeroAgeGet_Player*
CGSIHeroAgeGet_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIHeroAgeGet_Player::GetHero()
{
    return g_pHero;
}

CGSIHeroAgeGet_Node*
CGSIHeroAgeGet_Container::LowerBound(
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
        "FSE2_00899560_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIHeroAgeGet_InterfaceStorage interfaceStorage = {};
    CGSIHeroAgeGet_Manager manager;
    CGSIHeroAgeGet_Player player;
    CGSIHeroAgeGet_HeroStorage hero = {};
    CGSIHeroAgeGet_Component foundComponent = {};
    CGSIHeroAgeGet_Component endComponent = {};
    CGSIHeroAgeGet_Node foundNode = {};
    CGSIHeroAgeGet_Node endNode = {};

    interfaceStorage.m_pManager = &manager;
    interfaceStorage.m_FallbackAge = 21.5f;
    CGameScriptInterface* const pInterface =
        reinterpret_cast<CGameScriptInterface*>(
            &interfaceStorage);
    g_pPlayer = &player;
    foundNode.m_pComponent = &foundComponent;
    endNode.m_pComponent = &endComponent;
    hero.m_Container.m_pEnd = &endNode;

    ResetObservations();
    g_pHero = 0;
    if (pInterface->CGameScriptInterface::GetHeroAge() !=
            0.0f ||
        g_LowerBoundCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::GetHeroAge() !=
            0.0f ||
        g_LowerBoundCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    if (pInterface->CGameScriptInterface::GetHeroAge() !=
            21.5f ||
        g_LowerBoundCalls != 0)
        return Fail("fallback interface value");

    ResetObservations();
    hero.m_Flags20 = 0x10;
    foundNode.m_Key = 4;
    foundComponent.m_Age = 37.25f;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::GetHeroAge() !=
            37.25f ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 4 ||
        g_LowerBoundCalls != 1)
        return Fail("found HeroStats age");

    ResetObservations();
    foundNode.m_Key = 5;
    endComponent.m_Age = 49.75f;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::GetHeroAge() !=
        49.75f)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (pInterface->CGameScriptInterface::GetHeroAge() !=
        49.75f)
        return Fail("direct sentinel");

    std::printf("FSE2_00899560_TEST PASS\n");
    return 0;
}
