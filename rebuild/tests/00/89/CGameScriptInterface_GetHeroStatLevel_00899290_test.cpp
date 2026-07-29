#include <cstdio>

enum EHeroTrainableStatType
{
    EHeroTrainableStatType_First = 0,
    EHeroTrainableStatType_Test = 7
};

class CGSIHeroStatLevel_Player;
class CGSIHeroStatLevel_Node;

class CGSIHeroStatLevel_Manager
{
public:
    CGSIHeroStatLevel_Player* GetPlayer();
};

class CGSIHeroStatLevel_Player
{
public:
    void* GetHero();
};

class CGSIHeroStatLevel_Container
{
public:
    CGSIHeroStatLevel_Node* LowerBound(
        const long* pKey);

    CGSIHeroStatLevel_Node* m_pFirst;
    CGSIHeroStatLevel_Node* m_pEnd;
};

class CGSIHeroStatLevel_Component
{
public:
    long GetStatLevel(
        EHeroTrainableStatType statType);

    long m_Result;
};

class CGSIHeroStatLevel_Node
{
public:
    long m_Key;
    CGSIHeroStatLevel_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual long GetHeroStatLevel(
        EHeroTrainableStatType statType) const;
};

struct CGSIHeroStatLevel_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIHeroStatLevel_Manager* m_pManager;
};

struct CGSIHeroStatLevel_HeroStorage
{
    unsigned char m_Pad00[0x2C];
    unsigned long m_Flags2C;
    unsigned char m_Pad30[0x14];
    CGSIHeroStatLevel_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static CGSIHeroStatLevel_Player* g_pPlayer;
static CGSIHeroStatLevel_HeroStorage* g_pHero;
static CGSIHeroStatLevel_Node* g_pLowerBoundResult;
static CGSIHeroStatLevel_Component* g_pFallbackComponent;
static long g_FallbackResult;
static CGSIHeroStatLevel_Container* g_pObservedContainer;
static CGSIHeroStatLevel_Component* g_pObservedComponent;
static long g_ObservedKey;
static EHeroTrainableStatType g_ObservedStatType;
static unsigned long g_LowerBoundCalls;
static unsigned long g_GetStatLevelCalls;

CGSIHeroStatLevel_Player*
CGSIHeroStatLevel_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIHeroStatLevel_Player::GetHero()
{
    return g_pHero;
}

CGSIHeroStatLevel_Node*
CGSIHeroStatLevel_Container::LowerBound(
    const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

long CGSIHeroStatLevel_Component::GetStatLevel(
    EHeroTrainableStatType statType)
{
    g_pObservedComponent = this;
    g_ObservedStatType = statType;
    ++g_GetStatLevelCalls;
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
    g_GetStatLevelCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_00899290_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIHeroStatLevel_InterfaceStorage interfaceStorage = {};
    CGSIHeroStatLevel_Manager manager;
    CGSIHeroStatLevel_Player player;
    CGSIHeroStatLevel_HeroStorage hero = {};
    CGSIHeroStatLevel_Component fallbackComponent = {};
    CGSIHeroStatLevel_Component foundComponent = {};
    CGSIHeroStatLevel_Component endComponent = {};
    CGSIHeroStatLevel_Node foundNode = {};
    CGSIHeroStatLevel_Node endNode = {};

    interfaceStorage.m_pManager = &manager;
    CGameScriptInterface* const pInterface =
        reinterpret_cast<CGameScriptInterface*>(
            &interfaceStorage);
    g_pPlayer = &player;
    g_pFallbackComponent = &fallbackComponent;
    g_FallbackResult = 13;
    foundNode.m_pComponent = &foundComponent;
    endNode.m_pComponent = &endComponent;
    hero.m_Container.m_pEnd = &endNode;

    ResetObservations();
    g_pHero = 0;
    if (pInterface->CGameScriptInterface::
            GetHeroStatLevel(
                EHeroTrainableStatType_Test) != -1 ||
        g_GetStatLevelCalls != 0)
        return Fail("null hero guard");

    g_pHero = &hero;
    ResetObservations();
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            GetHeroStatLevel(
                EHeroTrainableStatType_Test) != -1 ||
        g_GetStatLevelCalls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags2C = 0;
    const EHeroTrainableStatType fallbackStat =
        static_cast<EHeroTrainableStatType>(
            reinterpret_cast<long>(
                &fallbackComponent));
    if (pInterface->CGameScriptInterface::
            GetHeroStatLevel(fallbackStat) !=
            g_FallbackResult ||
        g_LowerBoundCalls != 0 ||
        g_pObservedComponent !=
            &fallbackComponent ||
        g_ObservedStatType != fallbackStat)
        return Fail("fallback component dispatch");

    ResetObservations();
    hero.m_Flags2C = 0x100;
    foundNode.m_Key = 0x68;
    foundComponent.m_Result = 27;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroStatLevel(
                EHeroTrainableStatType_Test) != 27 ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 0x68 ||
        g_pObservedComponent != &foundComponent ||
        g_ObservedStatType !=
            EHeroTrainableStatType_Test ||
        g_LowerBoundCalls != 1 ||
        g_GetStatLevelCalls != 1)
        return Fail("found progression component");

    ResetObservations();
    foundNode.m_Key = 0x69;
    endComponent.m_Result = 41;
    g_pLowerBoundResult = &foundNode;
    if (pInterface->CGameScriptInterface::
            GetHeroStatLevel(
                EHeroTrainableStatType_Test) != 41 ||
        g_pObservedComponent != &endComponent)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (pInterface->CGameScriptInterface::
            GetHeroStatLevel(
                EHeroTrainableStatType_Test) != 41 ||
        g_pObservedComponent != &endComponent)
        return Fail("direct sentinel");

    std::printf("FSE2_00899290_TEST PASS\n");
    return 0;
}
