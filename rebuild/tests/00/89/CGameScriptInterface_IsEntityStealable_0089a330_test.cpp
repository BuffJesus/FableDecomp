#include <cstdio>

enum ETCInterfaceType { ETC_DUMMY = 0 };

struct CScriptThing;
struct CThingOverlay;

struct CScriptThingVTable
{
    unsigned char m_Pad00[0x2c];
    CThingOverlay* (__fastcall* m_Resolve)(CScriptThing* self);
    unsigned char m_Pad30[0xfc];
    bool (__fastcall* m_IsAvailable)(CScriptThing* self);
};

struct CScriptThing
{
    CScriptThingVTable* m_pVTable;
};

struct CGSIStealableComponent
{
    unsigned char m_Pad00[0x0d];
    bool          m_IsStealable;
};

struct CKeyPair
{
    ETCInterfaceType       m_Key;
    CGSIStealableComponent* m_Value;
};

struct CVectorMap
{
    CKeyPair* m_pBegin;
    CKeyPair* m_pEnd;
    CKeyPair* LowerBound(const ETCInterfaceType* pKey);
};

struct CThingOverlay
{
    unsigned char m_Pad00[0x28];
    unsigned int  m_Flags28;
    unsigned char m_Pad2C[0x18];
    CVectorMap    m_InterfaceMap44;
};

struct CGameScriptInterface
{
};

static bool g_available = false;
static CThingOverlay* g_thing = 0;
static unsigned long g_available_calls = 0;
static unsigned long g_resolve_calls = 0;
static CScriptThing* g_observed_script_thing = 0;
static CKeyPair* g_lower_bound_result = 0;
static CVectorMap* g_observed_map = 0;
static ETCInterfaceType g_observed_key = ETC_DUMMY;

static bool __fastcall IsAvailable(CScriptThing* self)
{
    g_observed_script_thing = self;
    ++g_available_calls;
    return g_available;
}

static CThingOverlay* __fastcall Resolve(CScriptThing* self)
{
    g_observed_script_thing = self;
    ++g_resolve_calls;
    return g_thing;
}

const ETCInterfaceType kTargetType = (ETCInterfaceType)0x54;

CKeyPair* CVectorMap::LowerBound(const ETCInterfaceType* pKey)
{
    g_observed_map = this;
    g_observed_key = *pKey;
    return g_lower_bound_result;
}

bool __fastcall CGameScriptInterface_IsEntityStealable(
    const CGameScriptInterface* self,
    int edxUnused,
    CScriptThing* pScriptThing);

static void ResetObservations()
{
    g_available_calls = 0;
    g_resolve_calls = 0;
    g_observed_script_thing = 0;
    g_lower_bound_result = 0;
    g_observed_map = 0;
    g_observed_key = ETC_DUMMY;
}

static int Fail(const char* message)
{
    std::printf("CGameScriptInterface_0089a330_TEST FAIL: %s\n", message);
    return 1;
}

int main()
{
    CGameScriptInterface gsi;
    CScriptThingVTable vtable = {};
    CScriptThing scriptThing = {};
    CThingOverlay thing = {};
    CGSIStealableComponent foundComponent = {};
    CGSIStealableComponent endComponent = {};
    CKeyPair foundEntry = {};
    CKeyPair greaterEntry = {};
    CKeyPair endEntry = {};

    vtable.m_Resolve = Resolve;
    vtable.m_IsAvailable = IsAvailable;
    scriptThing.m_pVTable = &vtable;
    g_thing = &thing;
    thing.m_InterfaceMap44.m_pEnd = &endEntry;
    foundEntry.m_Value = &foundComponent;
    endEntry.m_Value = &endComponent;

    ResetObservations();
    g_available = false;
    if (CGameScriptInterface_IsEntityStealable(
            &gsi, 0, &scriptThing) ||
        g_available_calls != 1 ||
        g_resolve_calls != 0 ||
        g_observed_script_thing != &scriptThing)
        return Fail("unavailable script thing");

    ResetObservations();
    g_available = true;
    thing.m_Flags28 = 0;
    if (CGameScriptInterface_IsEntityStealable(
            &gsi, 0, &scriptThing) ||
        g_resolve_calls != 1 ||
        g_observed_map != 0)
        return Fail("missing stealable capability");

    ResetObservations();
    thing.m_Flags28 = 0x100000;
    foundEntry.m_Key = (ETCInterfaceType)0x54;
    foundComponent.m_IsStealable = true;
    g_lower_bound_result = &foundEntry;
    if (!CGameScriptInterface_IsEntityStealable(
            &gsi, 0, &scriptThing) ||
        g_observed_map != &thing.m_InterfaceMap44 ||
        (int)g_observed_key != 0x54)
        return Fail("stealable component");

    ResetObservations();
    foundComponent.m_IsStealable = false;
    g_lower_bound_result = &foundEntry;
    if (CGameScriptInterface_IsEntityStealable(
            &gsi, 0, &scriptThing))
        return Fail("non-stealable component");

    ResetObservations();
    greaterEntry.m_Key = (ETCInterfaceType)0x55;
    greaterEntry.m_Value = &foundComponent;
    endComponent.m_IsStealable = true;
    g_lower_bound_result = &greaterEntry;
    if (!CGameScriptInterface_IsEntityStealable(
            &gsi, 0, &scriptThing))
        return Fail("greater-key sentinel");

    ResetObservations();
    g_lower_bound_result = &endEntry;
    if (!CGameScriptInterface_IsEntityStealable(
            &gsi, 0, &scriptThing))
        return Fail("direct sentinel");

    std::printf("CGameScriptInterface_0089a330_TEST PASS\n");
    return 0;
}
