#include <cstdio>

enum EHeroTrainableStatType
{
    EHeroTrainableStatType_First = 0,
    EHeroTrainableStatType_Test = 9
};

class CGSIHeroStatMax_Player;
class CGSIHeroStatMax_Node;

class CGSIHeroStatMax_Manager
{
public:
    CGSIHeroStatMax_Player* GetPlayer();
};

class CGSIHeroStatMax_Player
{
public:
    void* GetHero();
};

class CGSIHeroStatMax_Container
{
public:
    CGSIHeroStatMax_Node* LowerBound(
        const long* pKey);

    CGSIHeroStatMax_Node* m_pFirst;
    CGSIHeroStatMax_Node* m_pEnd;
};

class CGSIHeroStatMax_Component
{
public:
    long GetStatMax(
        EHeroTrainableStatType statType);

    long m_Result;
};

class CGSIHeroStatMax_Node
{
public:
    long m_Key;
    CGSIHeroStatMax_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual long GetHeroStatMax(
        EHeroTrainableStatType statType) const;
};

struct CGSIHeroStatMax_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIHeroStatMax_Manager* m_pManager;
};

struct CGSIHeroStatMax_HeroStorage
{
    unsigned char m_Pad00[0x2C];
    unsigned long m_Flags2C;
    unsigned char m_Pad30[0x14];
    CGSIHeroStatMax_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIHeroStatMax_Player* g_pPlayer;
static CGSIHeroStatMax_HeroStorage* g_pHero;
static CGSIHeroStatMax_Node* g_pLowerBoundResult;
static CGSIHeroStatMax_Component* g_pFallbackComponent;
static long g_FallbackResult;
static CGSIHeroStatMax_Container* g_pObservedContainer;
static CGSIHeroStatMax_Component* g_pObservedComponent;
static long g_ObservedKey;
static EHeroTrainableStatType g_ObservedStatType;
static unsigned long g_LowerBoundCalls;
static unsigned long g_GetStatMaxCalls;

CGSIHeroStatMax_Player*
CGSIHeroStatMax_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIHeroStatMax_Player::GetHero()
{
    return g_pHero;
}

CGSIHeroStatMax_Node*
CGSIHeroStatMax_Container::LowerBound(
    const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

long CGSIHeroStatMax_Component::GetStatMax(
    EHeroTrainableStatType statType)
{
    g_pObservedComponent = this;
    g_ObservedStatType = statType;
    ++g_GetStatMaxCalls;
    if (this == g_pFallbackComponent)
        return g_FallbackResult;
    return m_Result;
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_pObservedComponent = 0;
    g_ObservedKey = -1;
    g_ObservedStatType = EHeroTrainableStatType_First;
    g_LowerBoundCalls = 0;
    g_GetStatMaxCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_00899310_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIHeroStatMax_InterfaceStorage interfaceStorage = {};
    CGSIHeroStatMax_Manager manager;
    CGSIHeroStatMax_Player player;
    CGSIHeroStatMax_HeroStorage hero = {};
    CGSIHeroStatMax_Component fallbackComponent = {};
    CGSIHeroStatMax_Component foundComponent = {};
    CGSIHeroStatMax_Component endComponent = {};
    CGSIHeroStatMax_Node foundNode = {};
    CGSIHeroStatMax_Node endNode = {};

    interfaceStorage.m_pManager = &manager;
    CGameScriptInterface* const pInterface =
        reinterpret_cast<CGameScriptInterface*>(
            &interfaceStorage);
    g_pPlayer = &player;
    g_pFallbackComponent = &fallbackComponent;
    g_FallbackResult = 15;
    foundNode.m_pComponent = &foundComponent;
    endNode.m_pComponent = &endComponent;
    hero.m_Container.m_pEnd = &endNode;

    ResetObservations();
    g_pHero = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroStatMax(
                EHeroTrainableStatType_Test) != -1 ||
        g_GetStatMaxCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            GetHeroStatMax(
                EHeroTrainableStatType_Test) != -1 ||
        g_GetStatMaxCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags2C = 0;
    const EHeroTrainableStatType fallbackStat =
        static_cast<EHeroTrainableStatType>(
            reinterpret_cast<long>(
                &fallbackComponent));
    if (pInterface->CGameScriptInterface::
            GetHeroStatMax(fallbackStat) !=
            g_FallbackResult ||
        g_LowerBoundCalls != 0 ||
        g_pObservedComponent !=
            &fallbackComponent ||
        g_ObservedStatType != fallbackStat)
        return Fail("fallback component dispatch");

    ResetObservations();
    hero.m_Flags2C = 0x100;
    foundNode.m_Key = 0x68;
    foundComponent.m_Result = 31;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroStatMax(
                EHeroTrainableStatType_Test) != 31 ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 0x68 ||
        g_pObservedComponent != &foundComponent ||
        g_ObservedStatType !=
            EHeroTrainableStatType_Test ||
        g_LowerBoundCalls != 1 ||
        g_GetStatMaxCalls != 1)
        return Fail("found progression component");

    ResetObservations();
    foundNode.m_Key = 0x69;
    endComponent.m_Result = 47;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroStatMax(
                EHeroTrainableStatType_Test) != 47 ||
        g_pObservedComponent != &endComponent)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (pInterface->CGameScriptInterface::
            GetHeroStatMax(
                EHeroTrainableStatType_Test) != 47 ||
        g_pObservedComponent != &endComponent)
        return Fail("direct sentinel");

    std::printf("FSE2_00899310_TEST PASS\n");
    return 0;
}
