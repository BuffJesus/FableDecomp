#include <cstdio>

class CGSIHeroRenownLevel_Player;
class CGSIHeroRenownLevel_Node;

class CGSIHeroRenownLevel_Manager
{
public:
    CGSIHeroRenownLevel_Player* GetPlayer();
};

class CGSIHeroRenownLevel_Player
{
public:
    void* GetHero();
};

class CGSIHeroRenownLevel_Container
{
public:
    CGSIHeroRenownLevel_Node* LowerBound(
        const long* pKey);

    CGSIHeroRenownLevel_Node* m_pFirst;
    CGSIHeroRenownLevel_Node* m_pEnd;
};

class CGSIHeroRenownLevel_Component
{
public:
    unsigned char m_Pad00[0x70];
    long m_RenownLevel;
};

class CGSIHeroRenownLevel_Node
{
public:
    long m_Key;
    CGSIHeroRenownLevel_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual long GetHeroRenownLevel() const;
};

struct CGSIHeroRenownLevel_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIHeroRenownLevel_Manager* m_pManager;
    unsigned char m_Pad18[0x58];
    long m_FallbackRenownLevel;
};

struct CGSIHeroRenownLevel_HeroStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIHeroRenownLevel_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIHeroRenownLevel_Player* g_pPlayer;
static CGSIHeroRenownLevel_HeroStorage* g_pHero;
static CGSIHeroRenownLevel_Node* g_pLowerBoundResult;
static CGSIHeroRenownLevel_Container* g_pObservedContainer;
static long g_ObservedKey;
static unsigned long g_LowerBoundCalls;

CGSIHeroRenownLevel_Player*
CGSIHeroRenownLevel_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIHeroRenownLevel_Player::GetHero()
{
    return g_pHero;
}

CGSIHeroRenownLevel_Node*
CGSIHeroRenownLevel_Container::LowerBound(
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
        "FSE2_00899140_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIHeroRenownLevel_InterfaceStorage
        interfaceStorage = {};
    CGSIHeroRenownLevel_Manager manager;
    CGSIHeroRenownLevel_Player player;
    CGSIHeroRenownLevel_HeroStorage hero = {};
    CGSIHeroRenownLevel_Component foundComponent = {};
    CGSIHeroRenownLevel_Component endComponent = {};
    CGSIHeroRenownLevel_Node foundNode = {};
    CGSIHeroRenownLevel_Node endNode = {};

    interfaceStorage.m_pManager = &manager;
    interfaceStorage.m_FallbackRenownLevel = 3;
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
            GetHeroRenownLevel() != -1 ||
        g_LowerBoundCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            GetHeroRenownLevel() != -1 ||
        g_LowerBoundCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroRenownLevel() != 3 ||
        g_LowerBoundCalls != 0)
        return Fail("fallback interface value");

    ResetObservations();
    hero.m_Flags20 = 0x10;
    foundNode.m_Key = 4;
    foundComponent.m_RenownLevel = 6;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroRenownLevel() != 6 ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 4 ||
        g_LowerBoundCalls != 1)
        return Fail("found HeroStats value");

    ResetObservations();
    foundNode.m_Key = 5;
    endComponent.m_RenownLevel = 9;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroRenownLevel() != 9)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (pInterface->CGameScriptInterface::
            GetHeroRenownLevel() != 9)
        return Fail("direct sentinel");

    std::printf("FSE2_00899140_TEST PASS\n");
    return 0;
}
