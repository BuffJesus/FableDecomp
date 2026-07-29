#include <cstdio>

class CScriptThing
{
public:
    CScriptThing();
    void Construct();

    void** m_pVTable;
    unsigned long m_Field04;
    unsigned long m_Field08;
};

class CGSIHeroTarget_Player;

class CGSIHeroTarget_Manager
{
public:
    CGSIHeroTarget_Player* GetPlayer();
};

class CGSIHeroTarget_Player
{
public:
    void* GetHero();
};

class CGSIHeroTarget_Node;

class CGSIHeroTarget_Container
{
public:
    CGSIHeroTarget_Node* LowerBound(const long* pKey);

    CGSIHeroTarget_Node* m_pFirst;
    CGSIHeroTarget_Node* m_pEnd;
};

class CGSIHeroTarget_Thing
{
public:
    CScriptThing BuildScriptThing();
};

class CGSIHeroTarget_Provider
{
public:
    void** m_pVTable;
};

class CGSIHeroTarget_Node
{
public:
    long m_Key;
    CGSIHeroTarget_Provider* m_pProvider;
};

class CGameScriptInterface
{
public:
    virtual CScriptThing GetHeroTargetedThing() const;
};

struct InterfaceStorage
{
    void* m_pVTable;
    unsigned char m_Pad04[0x10];
    CGSIHeroTarget_Manager* m_pManager;
};

struct EntityStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIHeroTarget_Container m_Container;
};

static void* g_ResultVTable[34];
static void* g_ProviderVTable[34];
static CGSIHeroTarget_Player* g_pPlayer;
static EntityStorage* g_pHero;
static CGSIHeroTarget_Node* g_pLowerBoundResult;
static CGSIHeroTarget_Thing* g_pResolvedThing;
static CGSIHeroTarget_Manager* g_pObservedManager;
static CGSIHeroTarget_Player* g_pObservedPlayer;
static CGSIHeroTarget_Container* g_pObservedContainer;
static CGSIHeroTarget_Provider* g_pObservedProvider;
static CGSIHeroTarget_Thing* g_pObservedBuildThing;
static long g_ObservedKey;
static unsigned long g_ConstructCalls;
static unsigned long g_ManagerCalls;
static unsigned long g_PlayerCalls;
static unsigned long g_LowerBoundCalls;
static unsigned long g_ProviderCalls;
static unsigned long g_BuildCalls;

static CGSIHeroTarget_Thing* __fastcall ResolveProvider(
    CGSIHeroTarget_Provider* pProvider)
{
    g_pObservedProvider = pProvider;
    ++g_ProviderCalls;
    return g_pResolvedThing;
}

CScriptThing::CScriptThing()
{
    Construct();
}

void CScriptThing::Construct()
{
    m_pVTable = g_ResultVTable;
    m_Field04 = 0;
    m_Field08 = 0;
    ++g_ConstructCalls;
}

CGSIHeroTarget_Player* CGSIHeroTarget_Manager::GetPlayer()
{
    g_pObservedManager = this;
    ++g_ManagerCalls;
    return g_pPlayer;
}

void* CGSIHeroTarget_Player::GetHero()
{
    g_pObservedPlayer = this;
    ++g_PlayerCalls;
    return g_pHero;
}

CGSIHeroTarget_Node*
CGSIHeroTarget_Container::LowerBound(const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

CScriptThing CGSIHeroTarget_Thing::BuildScriptThing()
{
    g_pObservedBuildThing = this;
    ++g_BuildCalls;
    CScriptThing result;
    result.m_Field04 = 0x12345678;
    result.m_Field08 = 0x9ABCDEF0;
    return result;
}

static void __declspec(naked) CallInto(
    CGameScriptInterface*,
    CScriptThing*)
{
    __asm
    {
        mov ecx, dword ptr [esp + 4]
        push dword ptr [esp + 8]
        call CGameScriptInterface::GetHeroTargetedThing
        ret
    }
}

static void ResetObservations()
{
    g_pObservedManager = 0;
    g_pObservedPlayer = 0;
    g_pObservedContainer = 0;
    g_pObservedProvider = 0;
    g_pObservedBuildThing = 0;
    g_ObservedKey = 0;
    g_ConstructCalls = 0;
    g_ManagerCalls = 0;
    g_PlayerCalls = 0;
    g_LowerBoundCalls = 0;
    g_ProviderCalls = 0;
    g_BuildCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf("FSE2_0089a490_TEST FAIL: %s\n", pMessage);
    return 1;
}

static bool IsBuiltResult(const CScriptThing& result)
{
    return result.m_pVTable == g_ResultVTable &&
        result.m_Field04 == 0x12345678 &&
        result.m_Field08 == 0x9ABCDEF0;
}

int main()
{
    g_ResultVTable[33] =
        reinterpret_cast<void*>(ResolveProvider);
    g_ProviderVTable[33] =
        reinterpret_cast<void*>(ResolveProvider);

    InterfaceStorage interfaceStorage = {};
    CGSIHeroTarget_Manager manager;
    CGSIHeroTarget_Player player;
    EntityStorage hero = {};
    CGSIHeroTarget_Node foundNode = {};
    CGSIHeroTarget_Node endNode = {};
    CGSIHeroTarget_Provider foundProvider = {};
    CGSIHeroTarget_Provider endProvider = {};
    CGSIHeroTarget_Thing targetThing;
    CScriptThing result;

    interfaceStorage.m_pManager = &manager;
    CGameScriptInterface* const pInterface =
        reinterpret_cast<CGameScriptInterface*>(&interfaceStorage);
    g_pPlayer = &player;
    foundProvider.m_pVTable = g_ProviderVTable;
    endProvider.m_pVTable = g_ProviderVTable;
    foundNode.m_pProvider = &foundProvider;
    endNode.m_pProvider = &endProvider;
    hero.m_Container.m_pEnd = &endNode;

    ResetObservations();
    g_pHero = 0;
    result.m_Field04 = 1;
    result.m_Field08 = 2;
    CallInto(pInterface, &result);
    if (result.m_Field04 != 0 || result.m_Field08 != 0 ||
        g_ConstructCalls != 1 ||
        g_ManagerCalls != 1 || g_PlayerCalls != 1 ||
        g_LowerBoundCalls != 0 || g_ProviderCalls != 0 ||
        g_BuildCalls != 0)
        return Fail("null hero fallback");

    ResetObservations();
    g_pHero = &hero;
    hero.m_Flags20 = 0x100;
    foundNode.m_Key = 8;
    g_pLowerBoundResult = &foundNode;
    g_pResolvedThing = 0;
    CallInto(pInterface, &result);
    if (g_pObservedContainer != &hero.m_Container ||
        g_ObservedKey != 8 || g_pObservedProvider != &foundProvider ||
        g_ConstructCalls != 1 || g_LowerBoundCalls != 1 ||
        g_ProviderCalls != 1 || g_BuildCalls != 0)
        return Fail("found provider null target");

    ResetObservations();
    foundNode.m_Key = 9;
    g_pLowerBoundResult = &foundNode;
    g_pResolvedThing = &targetThing;
    CallInto(pInterface, &result);
    if (!IsBuiltResult(result) ||
        g_pObservedProvider != &endProvider ||
        g_pObservedBuildThing != &targetThing ||
        g_LowerBoundCalls != 1 || g_ProviderCalls != 1 ||
        g_BuildCalls != 1)
        return Fail("sentinel provider success");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    CallInto(pInterface, &result);
    if (!IsBuiltResult(result) ||
        g_pObservedProvider != &endProvider ||
        g_BuildCalls != 1)
        return Fail("direct sentinel");

    ResetObservations();
    hero.m_Flags20 = 0;
    result.m_pVTable = g_ResultVTable;
    CallInto(pInterface, &result);
    if (!IsBuiltResult(result) ||
        g_pObservedProvider !=
            reinterpret_cast<CGSIHeroTarget_Provider*>(&result) ||
        g_LowerBoundCalls != 0 || g_ProviderCalls != 1 ||
        g_pObservedBuildThing != &targetThing ||
        g_BuildCalls != 1)
        return Fail("result provider path");

    if (g_pObservedManager != &manager ||
        g_pObservedPlayer != &player)
        return Fail("manager/player dispatch");

    std::printf("FSE2_0089a490_TEST PASS\n");
    return 0;
}
