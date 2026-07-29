#include <cstdio>

enum EHeroAbility
{
    EHeroAbility_First = 0,
    EHeroAbility_Test = 3
};

class CGSIAbility_Player;
class CGSIAbility_Node;

class CGSIAbility_Manager
{
public:
    CGSIAbility_Player* GetPlayer();
};

class CGSIAbility_Player
{
public:
    void* GetHero();
};

class CGSIAbility_Container
{
public:
    CGSIAbility_Node* LowerBound(const long* pKey);

    CGSIAbility_Node* m_pFirst;
    CGSIAbility_Node* m_pEnd;
};

class CGSIAbility_Component
{
public:
    unsigned char m_Pad00[0x1C];
    unsigned char* m_pAvailability;
};

class CGSIAbility_Node
{
public:
    long m_Key;
    CGSIAbility_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual void SetAbilityAvailability(
        EHeroAbility ability,
        bool available) const;
};

struct InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIAbility_Manager* m_pManager;
};

struct EntityStorage
{
    unsigned char m_Pad00[0x34];
    unsigned char m_Flags34;
    unsigned char m_Pad35[0x0F];
    CGSIAbility_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIAbility_Player* g_pPlayer;
static EntityStorage* g_pHero;
static CGSIAbility_Node* g_pLowerBoundResult;
static CGSIAbility_Manager* g_pObservedManager;
static CGSIAbility_Player* g_pObservedPlayer;
static CGSIAbility_Container* g_pObservedContainer;
static long g_ObservedKey;
static unsigned long g_ManagerCalls;
static unsigned long g_PlayerCalls;
static unsigned long g_LowerBoundCalls;

CGSIAbility_Player* CGSIAbility_Manager::GetPlayer()
{
    g_pObservedManager = this;
    ++g_ManagerCalls;
    return g_pPlayer;
}

void* CGSIAbility_Player::GetHero()
{
    g_pObservedPlayer = this;
    ++g_PlayerCalls;
    return g_pHero;
}

CGSIAbility_Node*
CGSIAbility_Container::LowerBound(const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

static void ResetObservations()
{
    g_pObservedManager = 0;
    g_pObservedPlayer = 0;
    g_pObservedContainer = 0;
    g_ObservedKey = 0;
    g_ManagerCalls = 0;
    g_PlayerCalls = 0;
    g_LowerBoundCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_0089d7c0_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    InterfaceStorage interfaceStorage = {};
    CGSIAbility_Manager manager;
    CGSIAbility_Player player;
    EntityStorage hero = {};
    CGSIAbility_Component foundComponent = {};
    CGSIAbility_Component endComponent = {};
    CGSIAbility_Node foundNode = {};
    CGSIAbility_Node endNode = {};
    unsigned char foundAvailability[8] = {};
    unsigned char endAvailability[8] = {};

    interfaceStorage.m_pManager = &manager;
    CGameScriptInterface* const pInterface =
        reinterpret_cast<CGameScriptInterface*>(
            &interfaceStorage);
    g_pPlayer = &player;
    foundComponent.m_pAvailability = foundAvailability;
    endComponent.m_pAvailability = endAvailability;
    foundNode.m_pComponent = &foundComponent;
    endNode.m_pComponent = &endComponent;
    hero.m_Container.m_pEnd = &endNode;

    ResetObservations();
    g_pHero = 0;
    pInterface->CGameScriptInterface::
        SetAbilityAvailability(EHeroAbility_Test, true);
    if (g_ManagerCalls != 1 || g_PlayerCalls != 1 ||
        g_LowerBoundCalls != 0 ||
        foundAvailability[3] != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    hero.m_Flags34 = 0x80;

    ResetObservations();
    hero.m_Flags91 = 1;
    pInterface->CGameScriptInterface::
        SetAbilityAvailability(EHeroAbility_Test, true);
    if (g_LowerBoundCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags34 = 0x7F;
    pInterface->CGameScriptInterface::
        SetAbilityAvailability(EHeroAbility_Test, true);
    if (g_LowerBoundCalls != 0)
        return Fail("ability component guard");

    ResetObservations();
    hero.m_Flags34 = 0x80;
    foundNode.m_Key = 0xA7;
    g_pLowerBoundResult = &foundNode;
    foundAvailability[2] = 0x55;
    pInterface->CGameScriptInterface::
        SetAbilityAvailability(EHeroAbility_Test, true);
    if (g_pObservedManager != &manager ||
        g_pObservedPlayer != &player ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 0xA7 ||
        foundAvailability[3] != 1 ||
        foundAvailability[2] != 0x55)
        return Fail("found component enable");

    ResetObservations();
    foundNode.m_Key = 0xA8;
    g_pLowerBoundResult = &foundNode;
    endAvailability[3] = 1;
    pInterface->CGameScriptInterface::
        SetAbilityAvailability(EHeroAbility_Test, false);
    if (endAvailability[3] != 0 ||
        foundAvailability[3] != 1)
        return Fail("greater-key sentinel disable");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    pInterface->CGameScriptInterface::
        SetAbilityAvailability(EHeroAbility_Test, true);
    if (endAvailability[3] != 1)
        return Fail("direct sentinel enable");

    std::printf("FSE2_0089d7c0_TEST PASS\n");
    return 0;
}
