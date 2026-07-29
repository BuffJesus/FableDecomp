#include <cstdio>

enum ETCInterfaceType
{
    ETCInterfaceType_CombatAbility = 0x30
};

struct CCombatAbilityKeyPair
{
    ETCInterfaceType m_Key;
    void*             m_pValue;
};

struct CCombatAbilityMap
{
    CCombatAbilityKeyPair* m_pBegin;
    CCombatAbilityKeyPair* m_pEnd;
};

struct CThingOverlay
{
    unsigned char     m_Pad00[0x24];
    unsigned long     m_Flags24;
    unsigned char     m_Pad28[0x1C];
    CCombatAbilityMap m_InterfaceMap44;
    unsigned char     m_Pad4C[0x45];
    unsigned char     m_Flags91;
};

struct CScriptThing
{
    void** m_pVTable;
};

struct CGameScriptInterface
{
    void* m_pVTable;
};

static CThingOverlay* g_pResolvedThing = 0;
static CScriptThing* g_pResolveReceiver = 0;
static CCombatAbilityKeyPair* g_pLowerBoundResult = 0;
static CCombatAbilityMap* g_pObservedMap = 0;
static ETCInterfaceType g_ObservedKey = (ETCInterfaceType)0;
static void* g_pSetReceiver = 0;
static float g_SetAbility = 0.0f;
static int g_SetEnabled = 0;
static unsigned long g_ResolveCalls = 0;
static unsigned long g_LowerBoundCalls = 0;
static unsigned long g_SetCalls = 0;

extern "C" CThingOverlay* __fastcall
Sub_ResolveScriptThing(CScriptThing* self)
{
    g_pResolveReceiver = self;
    ++g_ResolveCalls;
    return g_pResolvedThing;
}

extern "C" CCombatAbilityKeyPair* __fastcall
Sub_FindCombatAbilityInterface(
    CCombatAbilityMap* pMap,
    int edxUnused,
    const ETCInterfaceType* pKey)
{
    g_pObservedMap = pMap;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

extern "C" void __fastcall Sub_SetCombatAbility(
    void* pInterface,
    int edxUnused,
    float ability,
    int enabled)
{
    g_pSetReceiver = pInterface;
    g_SetAbility = ability;
    g_SetEnabled = enabled;
    ++g_SetCalls;
}

void __fastcall CGameScriptInterface_EntitySetCombatAbility(
    const CGameScriptInterface* self,
    int edxUnused,
    const CScriptThing* pThing,
    float ability);

static void ResetObservations()
{
    g_pResolveReceiver = 0;
    g_pObservedMap = 0;
    g_ObservedKey = (ETCInterfaceType)0;
    g_pSetReceiver = 0;
    g_SetAbility = 0.0f;
    g_SetEnabled = 0;
    g_ResolveCalls = 0;
    g_LowerBoundCalls = 0;
    g_SetCalls = 0;
}

static int Fail(const char* message)
{
    std::printf("CGameScriptInterface_0089aff0_TEST FAIL: %s\n", message);
    return 1;
}

int main()
{
    CGameScriptInterface gsi = {};
    CScriptThing scriptThing = {};
    CThingOverlay thing = {};
    CCombatAbilityKeyPair foundEntry = {};
    CCombatAbilityKeyPair greaterEntry = {};
    CCombatAbilityKeyPair sentinelEntry = {};
    void* vtable[12] = {};
    int foundInterface = 1;
    int greaterInterface = 2;
    int sentinelInterface = 3;

    vtable[11] = (void*)&Sub_ResolveScriptThing;
    scriptThing.m_pVTable = vtable;
    foundEntry.m_Key = ETCInterfaceType_CombatAbility;
    foundEntry.m_pValue = &foundInterface;
    greaterEntry.m_Key = (ETCInterfaceType)0x31;
    greaterEntry.m_pValue = &greaterInterface;
    sentinelEntry.m_Key = (ETCInterfaceType)0x7FFFFFFF;
    sentinelEntry.m_pValue = &sentinelInterface;
    thing.m_InterfaceMap44.m_pEnd = &sentinelEntry;

    ResetObservations();
    g_pResolvedThing = 0;
    CGameScriptInterface_EntitySetCombatAbility(
        &gsi, 0, &scriptThing, 0.25f);
    if (g_ResolveCalls != 1 ||
        g_pResolveReceiver != &scriptThing ||
        g_LowerBoundCalls != 0 ||
        g_SetCalls != 0)
        return Fail("null thing guard");

    ResetObservations();
    g_pResolvedThing = &thing;
    thing.m_Flags91 = 1;
    CGameScriptInterface_EntitySetCombatAbility(
        &gsi, 0, &scriptThing, 0.25f);
    if (g_LowerBoundCalls != 0 || g_SetCalls != 0)
        return Fail("dead thing guard");

    ResetObservations();
    thing.m_Flags91 = 0;
    thing.m_Flags24 = 0;
    CGameScriptInterface_EntitySetCombatAbility(
        &gsi, 0, &scriptThing, 0.25f);
    if (g_LowerBoundCalls != 0 || g_SetCalls != 0)
        return Fail("component-map guard");

    ResetObservations();
    thing.m_Flags24 = 0x10000;
    g_pLowerBoundResult = &foundEntry;
    CGameScriptInterface_EntitySetCombatAbility(
        &gsi, 0, &scriptThing, -1.5f);
    if (g_LowerBoundCalls != 1 ||
        g_pObservedMap != &thing.m_InterfaceMap44 ||
        g_ObservedKey != ETCInterfaceType_CombatAbility ||
        g_SetCalls != 1 ||
        g_pSetReceiver != &foundInterface ||
        g_SetAbility != -1.5f ||
        g_SetEnabled != 1)
        return Fail("exact component dispatch");

    ResetObservations();
    g_pLowerBoundResult = &greaterEntry;
    CGameScriptInterface_EntitySetCombatAbility(
        &gsi, 0, &scriptThing, 3.75f);
    if (g_SetCalls != 1 ||
        g_pSetReceiver != &sentinelInterface ||
        g_SetAbility != 3.75f)
        return Fail("greater-key sentinel fallback");

    ResetObservations();
    g_pLowerBoundResult = &sentinelEntry;
    CGameScriptInterface_EntitySetCombatAbility(
        &gsi, 0, &scriptThing, 0.5f);
    if (g_SetCalls != 1 ||
        g_pSetReceiver != &sentinelInterface ||
        g_SetAbility != 0.5f)
        return Fail("end sentinel fallback");

    std::printf("CGameScriptInterface_0089aff0_TEST PASS\n");
    return 0;
}
