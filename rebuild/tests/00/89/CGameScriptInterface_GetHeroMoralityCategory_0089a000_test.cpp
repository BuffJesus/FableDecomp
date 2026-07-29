#include <cstdio>

enum EMorality
{
    EMorality_Good = 1,
    EMorality_Evil = 2,
    EMorality_Default = 3
};

class CGSIMorality_Player;
class CGSIMorality_Node;

class CGSIMorality_Manager
{
public:
    CGSIMorality_Player* GetPlayer();
};

class CGSIMorality_Player
{
public:
    void* GetHero();
};

class CGSIMorality_Container
{
public:
    CGSIMorality_Node* LowerBound(const long* pKey);

    CGSIMorality_Node* m_pFirst;
    CGSIMorality_Node* m_pEnd;
};

class CGSIMorality_Component
{
public:
    EMorality GetMoralityCategory();

    EMorality m_Result;
};

class CGSIMorality_Node
{
public:
    long m_Key;
    CGSIMorality_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual EMorality GetHeroMoralityCategory() const;
};

struct InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIMorality_Manager* m_pManager;
};

struct EntityStorage
{
    unsigned char m_Pad00[0x20];
    unsigned char m_Flags20;
    unsigned char m_Pad21[0x23];
    CGSIMorality_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIMorality_Player* g_pPlayer;
static EntityStorage* g_pHero;
static CGSIMorality_Node* g_pLowerBoundResult;
static void* g_pFallbackComponent;
static EMorality g_FallbackResult;
static CGSIMorality_Container* g_pObservedContainer;
static CGSIMorality_Component* g_pObservedComponent;
static long g_ObservedKey;
static unsigned long g_LowerBoundCalls;
static unsigned long g_QueryCalls;

CGSIMorality_Player* CGSIMorality_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIMorality_Player::GetHero()
{
    return g_pHero;
}

CGSIMorality_Node*
CGSIMorality_Container::LowerBound(const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

EMorality CGSIMorality_Component::GetMoralityCategory()
{
    g_pObservedComponent = this;
    ++g_QueryCalls;
    if (this == g_pFallbackComponent)
        return g_FallbackResult;
    return m_Result;
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_pObservedComponent = 0;
    g_ObservedKey = 0;
    g_LowerBoundCalls = 0;
    g_QueryCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_0089a000_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    InterfaceStorage interfaceStorage = {};
    CGSIMorality_Manager manager;
    CGSIMorality_Player player;
    EntityStorage hero = {};
    CGSIMorality_Component foundComponent = {};
    CGSIMorality_Component endComponent = {};
    CGSIMorality_Node foundNode = {};
    CGSIMorality_Node endNode = {};

    interfaceStorage.m_pManager = &manager;
    CGameScriptInterface* const pInterface =
        reinterpret_cast<CGameScriptInterface*>(
            &interfaceStorage);
    g_pFallbackComponent = &interfaceStorage;
    g_FallbackResult = EMorality_Evil;
    g_pPlayer = &player;
    foundNode.m_pComponent = &foundComponent;
    endNode.m_pComponent = &endComponent;
    hero.m_Container.m_pEnd = &endNode;

    g_pHero = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroMoralityCategory() != EMorality_Default)
        return Fail("null hero default");

    g_pHero = &hero;
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            GetHeroMoralityCategory() != EMorality_Default)
        return Fail("dead hero default");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroMoralityCategory() != EMorality_Evil ||
        g_pObservedComponent !=
            reinterpret_cast<CGSIMorality_Component*>(
                &interfaceStorage) ||
        g_LowerBoundCalls != 0 || g_QueryCalls != 1)
        return Fail("interface fallback component");

    ResetObservations();
    hero.m_Flags20 = 0x10;
    foundNode.m_Key = 4;
    foundComponent.m_Result = EMorality_Good;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroMoralityCategory() != EMorality_Good ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 4 ||
        g_pObservedComponent != &foundComponent)
        return Fail("found morality component");

    ResetObservations();
    foundNode.m_Key = 5;
    endComponent.m_Result = EMorality_Evil;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroMoralityCategory() != EMorality_Evil ||
        g_pObservedComponent != &endComponent)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (pInterface->CGameScriptInterface::
            GetHeroMoralityCategory() != EMorality_Evil)
        return Fail("direct sentinel");

    std::printf("FSE2_0089a000_TEST PASS\n");
    return 0;
}
