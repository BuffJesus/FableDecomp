#include <cstdio>
#include <cstring>

class CCharString
{
public:
    unsigned long m_Data;
};

class CGSIRemoveDead_String
{
public:
    void CopyConstruct(const void* pSource);
    void Destruct();

    const void* m_pValue;
};

class CGSIRemoveDead_SearchTools
{
public:
    bool GetPFirstThing(
        const void* pFilter,
        void** ppThing);
};

class CGSIRemoveDead_Thing
{
public:
    void Kill(bool immediately);
};

extern "C" void* __fastcall
CGSIRemoveDead_AppendCString(
    void* pDestination,
    const CCharString* pBase,
    const char* pSuffix);

class CGameScriptInterface
{
public:
    virtual void RemoveDeadCreature(
        const CCharString& creatureName) const;
};

struct CGSIRemoveDead_WorldStorage
{
    unsigned char m_Pad00[0x20];
    CGSIRemoveDead_SearchTools* m_pSearchTools;
};

struct CGSIRemoveDead_InterfaceStorage
{
    void* m_pVTable;
    CGSIRemoveDead_WorldStorage* m_pWorld;
};

static bool g_Found;
static CGSIRemoveDead_Thing* g_pThing;
static const CCharString* g_pObservedBase;
static const char* g_pObservedSuffix;
static void* g_pAppendDestination;
static void* g_pCopyDestination;
static const void* g_pCopySource;
static CGSIRemoveDead_SearchTools* g_pObservedSearchTools;
static const void* g_pObservedFilter;
static void** g_pObservedOutput;
static void* g_pDestructed[2];
static bool g_ObservedImmediately;
static unsigned long g_AppendCalls;
static unsigned long g_CopyCalls;
static unsigned long g_SearchCalls;
static unsigned long g_DestructCalls;
static unsigned long g_KillCalls;

extern "C" void* __fastcall
CGSIRemoveDead_AppendCString(
    void* pDestination,
    const CCharString* pBase,
    const char* pSuffix)
{
    g_pAppendDestination = pDestination;
    g_pObservedBase = pBase;
    g_pObservedSuffix = pSuffix;
    ++g_AppendCalls;
    return pDestination;
}

void CGSIRemoveDead_String::CopyConstruct(
    const void* pSource)
{
    g_pCopyDestination = this;
    g_pCopySource = pSource;
    m_pValue = pSource;
    ++g_CopyCalls;
}

void CGSIRemoveDead_String::Destruct()
{
    if (g_DestructCalls < 2)
        g_pDestructed[g_DestructCalls] = this;
    ++g_DestructCalls;
}

bool CGSIRemoveDead_SearchTools::GetPFirstThing(
    const void* pFilter,
    void** ppThing)
{
    g_pObservedSearchTools = this;
    g_pObservedFilter = pFilter;
    g_pObservedOutput = ppThing;
    ++g_SearchCalls;
    *ppThing = g_pThing;
    return g_Found;
}

void CGSIRemoveDead_Thing::Kill(bool immediately)
{
    g_ObservedImmediately = immediately;
    ++g_KillCalls;
}

static void ResetObservations()
{
    g_pObservedBase = 0;
    g_pObservedSuffix = 0;
    g_pAppendDestination = 0;
    g_pCopyDestination = 0;
    g_pCopySource = 0;
    g_pObservedSearchTools = 0;
    g_pObservedFilter = 0;
    g_pObservedOutput = 0;
    g_pDestructed[0] = 0;
    g_pDestructed[1] = 0;
    g_ObservedImmediately = true;
    g_AppendCalls = 0;
    g_CopyCalls = 0;
    g_SearchCalls = 0;
    g_DestructCalls = 0;
    g_KillCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_00894490_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    CGSIRemoveDead_InterfaceStorage interfaceStorage = {};
    CGSIRemoveDead_WorldStorage world = {};
    CGSIRemoveDead_SearchTools searchTools;
    CGSIRemoveDead_Thing thing;
    CCharString creatureName = {0x12345678};

    interfaceStorage.m_pWorld = &world;
    world.m_pSearchTools = &searchTools;
    CGameScriptInterface* const pInterface =
        reinterpret_cast<CGameScriptInterface*>(
            &interfaceStorage);

    ResetObservations();
    g_Found = false;
    g_pThing = &thing;
    pInterface->CGameScriptInterface::RemoveDeadCreature(
        creatureName);
    if (g_AppendCalls != 1 ||
        g_CopyCalls != 1 ||
        g_SearchCalls != 1 ||
        g_DestructCalls != 2 ||
        g_KillCalls != 0)
        return Fail("not-found call counts");
    if (g_pObservedBase != &creatureName ||
        g_pObservedSuffix == 0 ||
        std::strcmp(
            g_pObservedSuffix,
            "_DEAD_CREATURE") != 0)
        return Fail("suffix construction inputs");
    if (g_pCopySource != g_pAppendDestination ||
        g_pObservedFilter != g_pCopyDestination)
        return Fail("filter construction sequence");
    if (g_pObservedSearchTools != &searchTools ||
        g_pObservedOutput == 0)
        return Fail("world search dispatch");
    if (g_pDestructed[0] != g_pCopyDestination ||
        g_pDestructed[1] != g_pAppendDestination)
        return Fail("temporary cleanup order");

    ResetObservations();
    g_Found = true;
    pInterface->CGameScriptInterface::RemoveDeadCreature(
        creatureName);
    if (g_AppendCalls != 1 ||
        g_CopyCalls != 1 ||
        g_SearchCalls != 1 ||
        g_DestructCalls != 2 ||
        g_KillCalls != 1)
        return Fail("found call counts");
    if (g_ObservedImmediately)
        return Fail("found creature kill mode");

    std::printf("FSE2_00894490_TEST PASS\n");
    return 0;
}
