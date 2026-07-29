#include <cstdio>

class CScriptThing
{
};

class CGSIOnlyTarget_Player;
class CGSIOnlyTarget_Node;

class CGSIOnlyTarget_Manager
{
public:
    CGSIOnlyTarget_Player* GetPlayer();
};

class CGSIOnlyTarget_Player
{
public:
    void* GetHero();
};

class CGSIOnlyTarget_Container
{
public:
    CGSIOnlyTarget_Node* LowerBound(
        const long* pKey);

    CGSIOnlyTarget_Node* m_pFirst;
    CGSIOnlyTarget_Node* m_pEnd;
};

class CGSIOnlyTarget_Component
{
public:
    void SetOnlyTarget(void* pThing);
};

class CGSIOnlyTarget_Node
{
public:
    long m_Key;
    CGSIOnlyTarget_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual void SetPlayerCreatureOnlyTarget(
        const CScriptThing& thing) const;
};

struct CGSIOnlyTarget_ContextStorage
{
    unsigned char m_Pad00[0x1C];
    CGSIOnlyTarget_Manager* m_pManager;
};

struct CGSIOnlyTarget_InterfaceStorage
{
    void* m_pVTable;
    unsigned long m_Pad04;
    CGSIOnlyTarget_ContextStorage* m_pContext;
};

struct CGSIOnlyTarget_HeroStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIOnlyTarget_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

struct CGSIOnlyTarget_ScriptThingStorage
{
    void** m_pVTable;
};

static CGSIOnlyTarget_Player* g_pPlayer;
static CGSIOnlyTarget_HeroStorage* g_pHero;
static void* g_pResolvedThing;
static bool g_Available;
static CGSIOnlyTarget_Node* g_pLowerBoundResult;
static CGSIOnlyTarget_Container* g_pObservedContainer;
static CGSIOnlyTarget_Component* g_pObservedComponent;
static void* g_pObservedThing;
static long g_ObservedKey;
static unsigned long g_AvailableCalls;
static unsigned long g_ResolveCalls;
static unsigned long g_LowerBoundCalls;
static unsigned long g_SetCalls;

CGSIOnlyTarget_Player*
CGSIOnlyTarget_Manager::GetPlayer()
{
    return g_pPlayer;
}

void* CGSIOnlyTarget_Player::GetHero()
{
    return g_pHero;
}

CGSIOnlyTarget_Node*
CGSIOnlyTarget_Container::LowerBound(
    const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

void CGSIOnlyTarget_Component::SetOnlyTarget(
    void* pThing)
{
    g_pObservedComponent = this;
    g_pObservedThing = pThing;
    ++g_SetCalls;
}

static bool __fastcall IsAvailable(
    CGSIOnlyTarget_ScriptThingStorage*)
{
    ++g_AvailableCalls;
    return g_Available;
}

static void* __fastcall ResolveThing(
    CGSIOnlyTarget_ScriptThingStorage*)
{
    ++g_ResolveCalls;
    return g_pResolvedThing;
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_pObservedComponent = 0;
    g_pObservedThing = 0;
    g_ObservedKey = -1;
    g_AvailableCalls = 0;
    g_ResolveCalls = 0;
    g_LowerBoundCalls = 0;
    g_SetCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_00899e10_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    void* scriptThingVTable[76] = {};
    CGSIOnlyTarget_InterfaceStorage interfaceStorage = {};
    CGSIOnlyTarget_ContextStorage context = {};
    CGSIOnlyTarget_Manager manager;
    CGSIOnlyTarget_Player player;
    CGSIOnlyTarget_HeroStorage hero = {};
    CGSIOnlyTarget_ScriptThingStorage scriptThingStorage = {};
    CGSIOnlyTarget_Component foundComponent;
    CGSIOnlyTarget_Component endComponent;
    CGSIOnlyTarget_Node foundNode = {};
    CGSIOnlyTarget_Node endNode = {};
    unsigned long targetMarker = 0;

    scriptThingVTable[11] =
        reinterpret_cast<void*>(ResolveThing);
    scriptThingVTable[75] =
        reinterpret_cast<void*>(IsAvailable);
    scriptThingStorage.m_pVTable = scriptThingVTable;
    interfaceStorage.m_pContext = &context;
    context.m_pManager = &manager;
    CGameScriptInterface* const pInterface =
        reinterpret_cast<CGameScriptInterface*>(
            &interfaceStorage);
    const CScriptThing& scriptThing =
        reinterpret_cast<const CScriptThing&>(
            scriptThingStorage);
    g_pPlayer = &player;
    g_pHero = &hero;
    g_pResolvedThing = &targetMarker;
    foundNode.m_pComponent = &foundComponent;
    endNode.m_pComponent = &endComponent;
    hero.m_Container.m_pEnd = &endNode;

    ResetObservations();
    g_Available = false;
    pInterface->CGameScriptInterface::
        SetPlayerCreatureOnlyTarget(scriptThing);
    if (g_AvailableCalls != 1 ||
        g_ResolveCalls != 0 ||
        g_SetCalls != 0)
        return Fail("unavailable script thing guard");

    ResetObservations();
    g_Available = true;
    g_pHero = 0;
    pInterface->CGameScriptInterface::
        SetPlayerCreatureOnlyTarget(scriptThing);
    if (g_ResolveCalls != 0 || g_SetCalls != 0)
        return Fail("null player creature guard");

    ResetObservations();
    g_pHero = &hero;
    hero.m_Flags91 = 1;
    pInterface->CGameScriptInterface::
        SetPlayerCreatureOnlyTarget(scriptThing);
    if (g_ResolveCalls != 0 || g_SetCalls != 0)
        return Fail("dead player creature guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    pInterface->CGameScriptInterface::
        SetPlayerCreatureOnlyTarget(scriptThing);
    if (g_ResolveCalls != 1 ||
        g_LowerBoundCalls != 0 ||
        g_SetCalls != 0)
        return Fail("component capability guard");

    ResetObservations();
    hero.m_Flags20 = 0x100;
    foundNode.m_Key = 8;
    g_pLowerBoundResult = &foundNode;
    pInterface->CGameScriptInterface::
        SetPlayerCreatureOnlyTarget(scriptThing);
    if (g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 8 ||
        g_pObservedComponent != &foundComponent ||
        g_pObservedThing != &targetMarker ||
        g_LowerBoundCalls != 1 ||
        g_SetCalls != 1)
        return Fail("found only-target component");

    ResetObservations();
    foundNode.m_Key = 9;
    g_pLowerBoundResult = &foundNode;
    pInterface->CGameScriptInterface::
        SetPlayerCreatureOnlyTarget(scriptThing);
    if (g_pObservedComponent != &endComponent)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    pInterface->CGameScriptInterface::
        SetPlayerCreatureOnlyTarget(scriptThing);
    if (g_pObservedComponent != &endComponent)
        return Fail("direct sentinel");

    std::printf("FSE2_00899e10_TEST PASS\n");
    return 0;
}
