#include <cstdio>

enum ETutorialCategory
{
    ETutorialCategory_First = 0,
    ETutorialCategory_Test = 7
};

class CGSITutorial_Player;
class CGSITutorial_Node;

class CGSITutorial_Manager
{
public:
    CGSITutorial_Player* GetPlayer();
};

class CGSITutorial_Player
{
public:
    void* GetHero();
};

class CGSITutorial_Container
{
public:
    CGSITutorial_Node* LowerBound(const long* pKey);

    CGSITutorial_Node* m_pFirst;
    CGSITutorial_Node* m_pEnd;
};

class CGSITutorial_Component
{
public:
    bool GiveTutorial(
        ETutorialCategory category,
        bool force);
};

class CGSITutorial_Node
{
public:
    long m_Key;
    CGSITutorial_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual bool GiveHeroTutorial(
        ETutorialCategory category) const;
};

struct InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSITutorial_Manager* m_pManager;
};

struct EntityStorage
{
    unsigned char m_Pad00[0x24];
    unsigned long m_Flags24;
    unsigned char m_Pad28[0x1C];
    CGSITutorial_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSITutorial_Player* g_pPlayer;
static EntityStorage* g_pHero;
static CGSITutorial_Node* g_pLowerBoundResult;
static bool g_GiveResult;
static CGSITutorial_Container* g_pObservedContainer;
static CGSITutorial_Component* g_pObservedComponent;
static ETutorialCategory g_ObservedCategory;
static bool g_ObservedForce;
static long g_ObservedKey;
static unsigned long g_LowerBoundCalls;
static unsigned long g_GiveCalls;

CGSITutorial_Player* CGSITutorial_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSITutorial_Player::GetHero()
{
    return g_pHero;
}

CGSITutorial_Node*
CGSITutorial_Container::LowerBound(const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

bool CGSITutorial_Component::GiveTutorial(
    ETutorialCategory category,
    bool force)
{
    g_pObservedComponent = this;
    g_ObservedCategory = category;
    g_ObservedForce = force;
    ++g_GiveCalls;
    return g_GiveResult;
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_pObservedComponent = 0;
    g_ObservedCategory = ETutorialCategory_First;
    g_ObservedForce = true;
    g_ObservedKey = 0;
    g_LowerBoundCalls = 0;
    g_GiveCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_00898a50_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    InterfaceStorage interfaceStorage = {};
    CGSITutorial_Manager manager;
    CGSITutorial_Player player;
    EntityStorage hero = {};
    CGSITutorial_Component foundComponent;
    CGSITutorial_Component endComponent;
    CGSITutorial_Node foundNode = {};
    CGSITutorial_Node endNode = {};

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
            GiveHeroTutorial(ETutorialCategory_Test) ||
        g_LowerBoundCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    hero.m_Flags24 = 0x200;

    ResetObservations();
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            GiveHeroTutorial(ETutorialCategory_Test))
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags24 = 0;
    if (pInterface->CGameScriptInterface::
            GiveHeroTutorial(ETutorialCategory_Test))
        return Fail("tutorial capability guard");

    ResetObservations();
    hero.m_Flags24 = 0x200;
    foundNode.m_Key = 0x29;
    foundNode.m_pComponent = 0;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GiveHeroTutorial(ETutorialCategory_Test) ||
        g_GiveCalls != 0)
        return Fail("null component");
    foundNode.m_pComponent = &foundComponent;

    ResetObservations();
    g_GiveResult = false;
    if (pInterface->CGameScriptInterface::
            GiveHeroTutorial(ETutorialCategory_Test) ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 0x29 ||
        g_pObservedComponent != &foundComponent ||
        g_ObservedCategory != ETutorialCategory_Test ||
        g_ObservedForce ||
        g_GiveCalls != 1)
        return Fail("found component false");

    ResetObservations();
    g_GiveResult = true;
    if (!pInterface->CGameScriptInterface::
            GiveHeroTutorial(ETutorialCategory_Test))
        return Fail("found component true");

    ResetObservations();
    foundNode.m_Key = 0x2A;
    g_pLowerBoundResult = &foundNode;
    if (!pInterface->CGameScriptInterface::
            GiveHeroTutorial(ETutorialCategory_Test) ||
        g_pObservedComponent != &endComponent)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (!pInterface->CGameScriptInterface::
            GiveHeroTutorial(ETutorialCategory_Test))
        return Fail("direct sentinel");

    std::printf("FSE2_00898a50_TEST PASS\n");
    return 0;
}
