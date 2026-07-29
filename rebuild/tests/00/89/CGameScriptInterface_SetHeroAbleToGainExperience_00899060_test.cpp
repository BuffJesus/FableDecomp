#include <cstdio>

class CGSIGainExperience_Player;
class CGSIGainExperience_Node;

class CGSIGainExperience_Manager
{
public:
    CGSIGainExperience_Player* GetPlayer();
};

class CGSIGainExperience_Player
{
public:
    void* GetHero();
};

class CGSIGainExperience_Container
{
public:
    CGSIGainExperience_Node* LowerBound(
        const long* pKey);

    CGSIGainExperience_Node* m_pFirst;
    CGSIGainExperience_Node* m_pEnd;
};

class CGSIGainExperience_Component
{
public:
    unsigned char m_Pad00[0x3C];
    unsigned char m_AbleToGainExperience;
};

class CGSIGainExperience_Node
{
public:
    long m_Key;
    CGSIGainExperience_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual void SetHeroAbleToGainExperience(
        bool ableToGainExperience) const;
};

struct CGSIGainExperience_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIGainExperience_Manager* m_pManager;
};

struct CGSIGainExperience_HeroStorage
{
    unsigned char m_Pad00[0x2C];
    unsigned long m_Flags2C;
    unsigned char m_Pad30[0x14];
    CGSIGainExperience_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIGainExperience_Player* g_pPlayer;
static CGSIGainExperience_HeroStorage* g_pHero;
static CGSIGainExperience_Node* g_pLowerBoundResult;
static CGSIGainExperience_Container* g_pObservedContainer;
static long g_ObservedKey;
static unsigned long g_LowerBoundCalls;

CGSIGainExperience_Player*
CGSIGainExperience_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIGainExperience_Player::GetHero()
{
    return g_pHero;
}

CGSIGainExperience_Node*
CGSIGainExperience_Container::LowerBound(
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
        "FSE2_00899060_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIGainExperience_InterfaceStorage
        interfaceStorage = {};
    CGSIGainExperience_Manager manager;
    CGSIGainExperience_Player player;
    CGSIGainExperience_HeroStorage hero = {};
    CGSIGainExperience_Component foundComponent = {};
    CGSIGainExperience_Component endComponent = {};
    CGSIGainExperience_Node foundNode = {};
    CGSIGainExperience_Node endNode = {};

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
        SetHeroAbleToGainExperience(true);
    if (g_LowerBoundCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    pInterface->CGameScriptInterface::
        SetHeroAbleToGainExperience(true);
    if (g_LowerBoundCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags2C = 0;
    pInterface->CGameScriptInterface::
        SetHeroAbleToGainExperience(true);
    if (g_LowerBoundCalls != 0)
        return Fail("progression capability guard");

    ResetObservations();
    hero.m_Flags2C = 0x100;
    foundNode.m_Key = 0x68;
    foundComponent.m_AbleToGainExperience = 0;
    g_pLowerBoundResult = &foundNode;
    pInterface->CGameScriptInterface::
        SetHeroAbleToGainExperience(true);
    if (g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 0x68 ||
        g_LowerBoundCalls != 1 ||
        foundComponent.m_AbleToGainExperience != 1)
        return Fail("found component true");

    ResetObservations();
    pInterface->CGameScriptInterface::
        SetHeroAbleToGainExperience(false);
    if (foundComponent.m_AbleToGainExperience != 0)
        return Fail("found component false");

    ResetObservations();
    foundNode.m_Key = 0x69;
    endComponent.m_AbleToGainExperience = 0;
    g_pLowerBoundResult = &foundNode;
    pInterface->CGameScriptInterface::
        SetHeroAbleToGainExperience(true);
    if (endComponent.m_AbleToGainExperience != 1)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    pInterface->CGameScriptInterface::
        SetHeroAbleToGainExperience(false);
    if (endComponent.m_AbleToGainExperience != 0)
        return Fail("direct sentinel");

    std::printf("FSE2_00899060_TEST PASS\n");
    return 0;
}
