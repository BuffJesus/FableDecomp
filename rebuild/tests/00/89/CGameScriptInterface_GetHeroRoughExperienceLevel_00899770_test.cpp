#include <cstdio>

class CGSIRoughExperience_Player;
class CGSIRoughExperience_Node;

class CGSIRoughExperience_Manager
{
public:
    CGSIRoughExperience_Player* GetPlayer();
};

class CGSIRoughExperience_Player
{
public:
    void* GetHero();
};

class CGSIRoughExperience_Container
{
public:
    CGSIRoughExperience_Node* LowerBound(
        const long* pKey);

    CGSIRoughExperience_Node* m_pFirst;
    CGSIRoughExperience_Node* m_pEnd;
};

class CGSIRoughExperience_Component
{
public:
    long GetRoughExperienceLevel();

    long m_Value;
};

class CGSIRoughExperience_Node
{
public:
    long m_Key;
    CGSIRoughExperience_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual long GetHeroRoughExperienceLevel() const;
};

struct InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIRoughExperience_Manager* m_pManager;
};

struct EntityStorage
{
    unsigned char m_Pad00[0x2C];
    unsigned long m_Flags2C;
    unsigned char m_Pad30[0x14];
    CGSIRoughExperience_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIRoughExperience_Player* g_pPlayer;
static EntityStorage* g_pHero;
static CGSIRoughExperience_Node* g_pLowerBoundResult;
static void* g_pFallbackComponent;
static long g_FallbackValue;
static CGSIRoughExperience_Container* g_pObservedContainer;
static CGSIRoughExperience_Component* g_pObservedComponent;
static long g_ObservedKey;
static unsigned long g_ManagerCalls;
static unsigned long g_PlayerCalls;
static unsigned long g_LowerBoundCalls;
static unsigned long g_QueryCalls;

CGSIRoughExperience_Player*
CGSIRoughExperience_Manager::GetPlayer()
{
    ++g_ManagerCalls;
    return g_pPlayer;
}

void* CGSIRoughExperience_Player::GetHero()
{
    ++g_PlayerCalls;
    return g_pHero;
}

CGSIRoughExperience_Node*
CGSIRoughExperience_Container::LowerBound(
    const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

long CGSIRoughExperience_Component::
    GetRoughExperienceLevel()
{
    g_pObservedComponent = this;
    ++g_QueryCalls;
    if (this == g_pFallbackComponent)
        return g_FallbackValue;
    return m_Value;
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_pObservedComponent = 0;
    g_ObservedKey = 0;
    g_ManagerCalls = 0;
    g_PlayerCalls = 0;
    g_LowerBoundCalls = 0;
    g_QueryCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_00899770_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    InterfaceStorage interfaceStorage = {};
    CGSIRoughExperience_Manager manager;
    CGSIRoughExperience_Player player;
    EntityStorage hero = {};
    CGSIRoughExperience_Component foundComponent = {};
    CGSIRoughExperience_Component endComponent = {};
    CGSIRoughExperience_Node foundNode = {};
    CGSIRoughExperience_Node endNode = {};

    interfaceStorage.m_pManager = &manager;
    CGameScriptInterface* const pInterface =
        reinterpret_cast<CGameScriptInterface*>(
            &interfaceStorage);
    g_pFallbackComponent = &interfaceStorage;
    g_FallbackValue = 77;
    g_pPlayer = &player;
    foundNode.m_pComponent = &foundComponent;
    endNode.m_pComponent = &endComponent;
    hero.m_Container.m_pEnd = &endNode;

    ResetObservations();
    g_pHero = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroRoughExperienceLevel() != 0 ||
        g_ManagerCalls != 1 || g_PlayerCalls != 1 ||
        g_QueryCalls != 0)
        return Fail("null hero guard");

    ResetObservations();
    g_pHero = &hero;
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            GetHeroRoughExperienceLevel() != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags2C = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroRoughExperienceLevel() != 77 ||
        g_pObservedComponent !=
            reinterpret_cast<
                CGSIRoughExperience_Component*>(
                    &interfaceStorage) ||
        g_LowerBoundCalls != 0 || g_QueryCalls != 1)
        return Fail("interface fallback component");

    ResetObservations();
    hero.m_Flags2C = 0x100;
    foundNode.m_Key = 0x68;
    foundComponent.m_Value = 135;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroRoughExperienceLevel() != 135 ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 0x68 ||
        g_pObservedComponent != &foundComponent)
        return Fail("found component value");

    ResetObservations();
    foundNode.m_pComponent = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroRoughExperienceLevel() != 0 ||
        g_QueryCalls != 0)
        return Fail("null component");
    foundNode.m_pComponent = &foundComponent;

    ResetObservations();
    foundNode.m_Key = 0x69;
    endComponent.m_Value = -246;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroRoughExperienceLevel() != -246 ||
        g_pObservedComponent != &endComponent)
        return Fail("greater-key sentinel value");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (pInterface->CGameScriptInterface::
            GetHeroRoughExperienceLevel() != -246)
        return Fail("direct sentinel value");

    std::printf("FSE2_00899770_TEST PASS\n");
    return 0;
}
