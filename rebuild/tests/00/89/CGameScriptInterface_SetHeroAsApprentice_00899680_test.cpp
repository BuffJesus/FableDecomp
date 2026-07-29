#include <cstdio>

class CGSIHeroApprentice_Player;
class CGSIHeroApprentice_Node;

class CGSIHeroApprentice_Manager
{
public:
    CGSIHeroApprentice_Player* GetPlayer();
};

class CGSIHeroApprentice_Player
{
public:
    void* GetHero();
};

class CGSIHeroApprentice_Container
{
public:
    CGSIHeroApprentice_Node* LowerBound(
        const long* pKey);

    CGSIHeroApprentice_Node* m_pFirst;
    CGSIHeroApprentice_Node* m_pEnd;
};

class CGSIHeroApprentice_Component
{
public:
    void ResetTrainingStatus();

    unsigned char m_Pad00[0x114];
    unsigned long m_TrainingStatus;
};

class CGSIHeroApprentice_Node
{
public:
    long m_Key;
    CGSIHeroApprentice_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual void SetHeroAsApprentice(
        bool apprentice) const;
};

struct CGSIHeroApprentice_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIHeroApprentice_Manager* m_pManager;
};

struct CGSIHeroApprentice_HeroStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIHeroApprentice_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIHeroApprentice_Player* g_pPlayer;
static CGSIHeroApprentice_HeroStorage* g_pHero;
static CGSIHeroApprentice_Node* g_pLowerBoundResult;
static CGSIHeroApprentice_Container* g_pObservedContainer;
static CGSIHeroApprentice_Component* g_pObservedResetComponent;
static long g_ObservedKey;
static unsigned long g_LowerBoundCalls;
static unsigned long g_ResetCalls;

CGSIHeroApprentice_Player*
CGSIHeroApprentice_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIHeroApprentice_Player::GetHero()
{
    return g_pHero;
}

CGSIHeroApprentice_Node*
CGSIHeroApprentice_Container::LowerBound(
    const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

void CGSIHeroApprentice_Component::
ResetTrainingStatus()
{
    g_pObservedResetComponent = this;
    ++g_ResetCalls;
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_pObservedResetComponent = 0;
    g_ObservedKey = -1;
    g_LowerBoundCalls = 0;
    g_ResetCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_00899680_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIHeroApprentice_InterfaceStorage
        interfaceStorage = {};
    CGSIHeroApprentice_Manager manager;
    CGSIHeroApprentice_Player player;
    CGSIHeroApprentice_HeroStorage hero = {};
    CGSIHeroApprentice_Component foundComponent = {};
    CGSIHeroApprentice_Component endComponent = {};
    CGSIHeroApprentice_Node foundNode = {};
    CGSIHeroApprentice_Node endNode = {};

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
        SetHeroAsApprentice(true);
    if (g_LowerBoundCalls != 0 || g_ResetCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    pInterface->CGameScriptInterface::
        SetHeroAsApprentice(true);
    if (g_LowerBoundCalls != 0 || g_ResetCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    pInterface->CGameScriptInterface::
        SetHeroAsApprentice(false);
    if (g_LowerBoundCalls != 0 ||
        g_ResetCalls != 1 ||
        g_pObservedResetComponent != 0)
        return Fail("false fallback reset dispatch");

    ResetObservations();
    hero.m_Flags20 = 0x10;
    foundNode.m_Key = 4;
    foundComponent.m_TrainingStatus = 0;
    g_pLowerBoundResult = &foundNode;
    pInterface->CGameScriptInterface::
        SetHeroAsApprentice(true);
    if (g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 4 ||
        foundComponent.m_TrainingStatus != 1 ||
        g_LowerBoundCalls != 1 ||
        g_ResetCalls != 0)
        return Fail("found component true write");

    ResetObservations();
    foundComponent.m_TrainingStatus = 9;
    pInterface->CGameScriptInterface::
        SetHeroAsApprentice(false);
    if (g_pObservedResetComponent != &foundComponent ||
        g_ResetCalls != 1 ||
        foundComponent.m_TrainingStatus != 9)
        return Fail("found component false reset");

    ResetObservations();
    foundNode.m_Key = 5;
    endComponent.m_TrainingStatus = 0;
    g_pLowerBoundResult = &foundNode;
    pInterface->CGameScriptInterface::
        SetHeroAsApprentice(true);
    if (endComponent.m_TrainingStatus != 1)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    pInterface->CGameScriptInterface::
        SetHeroAsApprentice(false);
    if (g_pObservedResetComponent != &endComponent)
        return Fail("direct sentinel");

    std::printf("FSE2_00899680_TEST PASS\n");
    return 0;
}
