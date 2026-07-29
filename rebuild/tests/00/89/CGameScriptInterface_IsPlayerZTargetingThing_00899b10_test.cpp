#include <cstdio>

class CScriptThing
{
};

class CGSIZTarget_Player;
class CGSIZTarget_Node;

class CGSIZTarget_Manager
{
public:
    CGSIZTarget_Player* GetPlayer();
};

class CGSIZTarget_Player
{
public:
    void* GetHero();
};

class CGSIZTarget_Container
{
public:
    CGSIZTarget_Node* LowerBound(
        const long* pKey);

    CGSIZTarget_Node* m_pFirst;
    CGSIZTarget_Node* m_pEnd;
};

class CGameScriptInterface
{
public:
    virtual bool IsPlayerZTargetingThing(
        const CScriptThing& thing) const;
};

struct CGSIZTarget_InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIZTarget_Manager* m_pManager;
};

struct CGSIZTarget_PlayerStorage
{
    unsigned char m_Pad00[0x20A];
    unsigned char m_ZTargetingActive;
};

struct CGSIZTarget_HeroStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIZTarget_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

struct CGSIZTarget_EntityStorage
{
    unsigned char m_Pad00[0x91];
    unsigned char m_Flags91;
};

struct CGSIZTarget_ScriptThingStorage
{
    void** m_pVTable;
};

struct CGSIZTarget_ComponentStorage
{
    void** m_pVTable;
};

class CGSIZTarget_Node
{
public:
    long m_Key;
    CGSIZTarget_ComponentStorage* m_pComponent;
};

static CGSIZTarget_PlayerStorage* g_pPlayerStorage;
static CGSIZTarget_HeroStorage* g_pHero;
static CGSIZTarget_EntityStorage* g_pResolvedThing;
static CGSIZTarget_EntityStorage* g_pComponentTarget;
static CGSIZTarget_Node* g_pLowerBoundResult;
static CGSIZTarget_Container* g_pObservedContainer;
static CGSIZTarget_ComponentStorage* g_pObservedComponent;
static long g_ObservedKey;
static unsigned long g_ResolveCalls;
static unsigned long g_LowerBoundCalls;
static unsigned long g_GetTargetCalls;

CGSIZTarget_Player* CGSIZTarget_Manager::GetPlayer()
{
    return reinterpret_cast<CGSIZTarget_Player*>(
        g_pPlayerStorage);
}

void* CGSIZTarget_Player::GetHero()
{
    return g_pHero;
}

CGSIZTarget_Node*
CGSIZTarget_Container::LowerBound(const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

static CGSIZTarget_EntityStorage* __fastcall
ResolveScriptThing(CGSIZTarget_ScriptThingStorage*)
{
    ++g_ResolveCalls;
    return g_pResolvedThing;
}

static CGSIZTarget_EntityStorage* __fastcall
GetComponentTarget(CGSIZTarget_ComponentStorage* pThis)
{
    g_pObservedComponent = pThis;
    ++g_GetTargetCalls;
    return g_pComponentTarget;
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_pObservedComponent = 0;
    g_ObservedKey = -1;
    g_ResolveCalls = 0;
    g_LowerBoundCalls = 0;
    g_GetTargetCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_00899b10_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    void* scriptThingVTable[12] = {};
    void* componentVTable[34] = {};
    CGSIZTarget_InterfaceStorage interfaceStorage = {};
    CGSIZTarget_Manager manager;
    CGSIZTarget_PlayerStorage player = {};
    CGSIZTarget_HeroStorage hero = {};
    CGSIZTarget_EntityStorage target = {};
    CGSIZTarget_EntityStorage other = {};
    CGSIZTarget_ScriptThingStorage scriptThingStorage = {};
    CGSIZTarget_ComponentStorage foundComponent = {};
    CGSIZTarget_ComponentStorage endComponent = {};
    CGSIZTarget_Node foundNode = {};
    CGSIZTarget_Node endNode = {};

    scriptThingVTable[11] =
        reinterpret_cast<void*>(ResolveScriptThing);
    componentVTable[33] =
        reinterpret_cast<void*>(GetComponentTarget);
    scriptThingStorage.m_pVTable = scriptThingVTable;
    foundComponent.m_pVTable = componentVTable;
    endComponent.m_pVTable = componentVTable;
    interfaceStorage.m_pManager = &manager;
    CGameScriptInterface* const pInterface =
        reinterpret_cast<CGameScriptInterface*>(
            &interfaceStorage);
    const CScriptThing& scriptThing =
        reinterpret_cast<const CScriptThing&>(
            scriptThingStorage);
    g_pPlayerStorage = &player;
    g_pHero = &hero;
    g_pResolvedThing = &target;
    foundNode.m_pComponent = &foundComponent;
    endNode.m_pComponent = &endComponent;
    hero.m_Container.m_pEnd = &endNode;

    ResetObservations();
    player.m_ZTargetingActive = 0;
    if (pInterface->CGameScriptInterface::
            IsPlayerZTargetingThing(scriptThing) ||
        g_ResolveCalls != 1 ||
        g_LowerBoundCalls != 0)
        return Fail("inactive targeting guard");

    ResetObservations();
    player.m_ZTargetingActive = 1;
    g_pResolvedThing = 0;
    if (pInterface->CGameScriptInterface::
            IsPlayerZTargetingThing(scriptThing))
        return Fail("null script target guard");

    ResetObservations();
    g_pResolvedThing = &target;
    target.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            IsPlayerZTargetingThing(scriptThing))
        return Fail("dead script target guard");

    ResetObservations();
    target.m_Flags91 = 0;
    g_pHero = 0;
    if (pInterface->CGameScriptInterface::
            IsPlayerZTargetingThing(scriptThing))
        return Fail("null hero guard");

    ResetObservations();
    g_pHero = &hero;
    hero.m_Flags91 = 1;
    if (pInterface->CGameScriptInterface::
            IsPlayerZTargetingThing(scriptThing))
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    if (pInterface->CGameScriptInterface::
            IsPlayerZTargetingThing(scriptThing))
        return Fail("targeting capability guard");

    ResetObservations();
    hero.m_Flags20 = 0x100;
    foundNode.m_Key = 8;
    g_pLowerBoundResult = &foundNode;
    g_pComponentTarget = &target;
    if (!pInterface->CGameScriptInterface::
            IsPlayerZTargetingThing(scriptThing) ||
        g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 8 ||
        g_pObservedComponent != &foundComponent ||
        g_LowerBoundCalls != 1 ||
        g_GetTargetCalls != 1)
        return Fail("matching component target");

    ResetObservations();
    g_pComponentTarget = &other;
    if (pInterface->CGameScriptInterface::
            IsPlayerZTargetingThing(scriptThing))
        return Fail("nonmatching component target");

    ResetObservations();
    foundNode.m_Key = 9;
    g_pLowerBoundResult = &foundNode;
    g_pComponentTarget = &target;
    if (!pInterface->CGameScriptInterface::
            IsPlayerZTargetingThing(scriptThing) ||
        g_pObservedComponent != &endComponent)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (!pInterface->CGameScriptInterface::
            IsPlayerZTargetingThing(scriptThing) ||
        g_pObservedComponent != &endComponent)
        return Fail("direct sentinel");

    std::printf("FSE2_00899b10_TEST PASS\n");
    return 0;
}
