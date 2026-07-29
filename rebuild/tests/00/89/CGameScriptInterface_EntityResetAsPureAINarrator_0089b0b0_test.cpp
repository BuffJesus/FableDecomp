#include <cstdio>

enum ETCInterfaceType
{
    ETCInterfaceType_Narrator = 0x0F
};

struct CNarratorComponent
{
    unsigned char m_Pad00[0x34];
    unsigned char m_ResetAsPureAI;
};

struct CNarratorKeyPair
{
    ETCInterfaceType    m_Key;
    CNarratorComponent* m_pValue;
};

struct CNarratorMap
{
    CNarratorKeyPair* m_pBegin;
    CNarratorKeyPair* m_pEnd;
};

struct CThingOverlay
{
    unsigned char m_Pad00[0x10];
    unsigned char m_EntityType10;
    unsigned char m_Pad11[0x0F];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CNarratorMap  m_InterfaceMap44;
};

struct CScriptThing
{
    void** m_pVTable;
};

struct CGameScriptInterface
{
    void* m_pVTable;
};

static bool g_IsValid = false;
static CThingOverlay* g_pResolvedThing = 0;
static CScriptThing* g_pValidityReceiver = 0;
static CScriptThing* g_pResolveReceiver = 0;
static CNarratorKeyPair* g_pLowerBoundResult = 0;
static CNarratorMap* g_pObservedMap = 0;
static ETCInterfaceType g_ObservedKey = (ETCInterfaceType)0;
static unsigned long g_ValidityCalls = 0;
static unsigned long g_ResolveCalls = 0;
static unsigned long g_LowerBoundCalls = 0;

extern "C" bool __fastcall Sub_IsValidScriptThing(CScriptThing* self)
{
    g_pValidityReceiver = self;
    ++g_ValidityCalls;
    return g_IsValid;
}

extern "C" CThingOverlay* __fastcall
Sub_ResolveNarratorThing(CScriptThing* self)
{
    g_pResolveReceiver = self;
    ++g_ResolveCalls;
    return g_pResolvedThing;
}

extern "C" CNarratorKeyPair* __fastcall Sub_FindNarratorInterface(
    CNarratorMap* pMap,
    int edxUnused,
    const ETCInterfaceType* pKey)
{
    g_pObservedMap = pMap;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

void __fastcall CGameScriptInterface_EntityResetAsPureAINarrator(
    const CGameScriptInterface* self,
    int edxUnused,
    const CScriptThing* pThing);

static void ResetObservations()
{
    g_pValidityReceiver = 0;
    g_pResolveReceiver = 0;
    g_pObservedMap = 0;
    g_ObservedKey = (ETCInterfaceType)0;
    g_ValidityCalls = 0;
    g_ResolveCalls = 0;
    g_LowerBoundCalls = 0;
}

static int Fail(const char* message)
{
    std::printf("CGameScriptInterface_0089b0b0_TEST FAIL: %s\n", message);
    return 1;
}

int main()
{
    CGameScriptInterface gsi = {};
    CScriptThing scriptThing = {};
    CThingOverlay thing = {};
    CNarratorComponent foundComponent = {};
    CNarratorComponent greaterComponent = {};
    CNarratorComponent sentinelComponent = {};
    CNarratorKeyPair foundEntry = {};
    CNarratorKeyPair greaterEntry = {};
    CNarratorKeyPair sentinelEntry = {};
    void* vtable[76] = {};

    vtable[75] = (void*)&Sub_IsValidScriptThing;
    vtable[11] = (void*)&Sub_ResolveNarratorThing;
    scriptThing.m_pVTable = vtable;
    foundEntry.m_Key = ETCInterfaceType_Narrator;
    foundEntry.m_pValue = &foundComponent;
    greaterEntry.m_Key = (ETCInterfaceType)0x10;
    greaterEntry.m_pValue = &greaterComponent;
    sentinelEntry.m_Key = (ETCInterfaceType)0x7FFFFFFF;
    sentinelEntry.m_pValue = &sentinelComponent;
    thing.m_InterfaceMap44.m_pEnd = &sentinelEntry;

    ResetObservations();
    g_IsValid = false;
    CGameScriptInterface_EntityResetAsPureAINarrator(
        &gsi, 0, &scriptThing);
    if (g_ValidityCalls != 1 ||
        g_pValidityReceiver != &scriptThing ||
        g_ResolveCalls != 0 ||
        g_LowerBoundCalls != 0)
        return Fail("invalid script thing guard");

    ResetObservations();
    g_IsValid = true;
    g_pResolvedThing = &thing;
    thing.m_EntityType10 = 0;
    CGameScriptInterface_EntityResetAsPureAINarrator(
        &gsi, 0, &scriptThing);
    if (g_ResolveCalls != 1 ||
        g_pResolveReceiver != &scriptThing ||
        g_LowerBoundCalls != 0)
        return Fail("entity type guard");

    ResetObservations();
    thing.m_EntityType10 = 1;
    thing.m_Flags20 = 0;
    CGameScriptInterface_EntityResetAsPureAINarrator(
        &gsi, 0, &scriptThing);
    if (g_LowerBoundCalls != 0)
        return Fail("narrator-map guard");

    ResetObservations();
    thing.m_Flags20 = 0x8000;
    g_pLowerBoundResult = &foundEntry;
    CGameScriptInterface_EntityResetAsPureAINarrator(
        &gsi, 0, &scriptThing);
    if (g_LowerBoundCalls != 1 ||
        g_pObservedMap != &thing.m_InterfaceMap44 ||
        g_ObservedKey != ETCInterfaceType_Narrator ||
        foundComponent.m_ResetAsPureAI != 1 ||
        sentinelComponent.m_ResetAsPureAI != 0)
        return Fail("exact narrator component");

    ResetObservations();
    g_pLowerBoundResult = &greaterEntry;
    CGameScriptInterface_EntityResetAsPureAINarrator(
        &gsi, 0, &scriptThing);
    if (sentinelComponent.m_ResetAsPureAI != 1 ||
        greaterComponent.m_ResetAsPureAI != 0)
        return Fail("greater-key sentinel fallback");

    sentinelComponent.m_ResetAsPureAI = 0;
    ResetObservations();
    g_pLowerBoundResult = &sentinelEntry;
    CGameScriptInterface_EntityResetAsPureAINarrator(
        &gsi, 0, &scriptThing);
    if (sentinelComponent.m_ResetAsPureAI != 1)
        return Fail("end sentinel fallback");

    std::printf("CGameScriptInterface_0089b0b0_TEST PASS\n");
    return 0;
}
