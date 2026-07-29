#include <cstdio>

enum ETCInterfaceType { ETC_DUMMY = 0 };

class CGSIOnlyTarget_Component
{
public:
    void SetOnlyTarget(void* pThing);
};

struct CKeyPair
{
    ETCInterfaceType          m_Key;
    CGSIOnlyTarget_Component* m_Value;
};

struct CVectorMap
{
    CKeyPair* m_pBegin;
    CKeyPair* m_pEnd;
    CKeyPair* LowerBound(const ETCInterfaceType* pKey);
};

struct CThingOverlay
{
    unsigned char m_Pad00[0x20];
    unsigned int  m_Flags20;
    unsigned char m_Pad24[0x20];
    CVectorMap    m_InterfaceMap44;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

struct CGameScriptContext
{
    unsigned char m_Pad00[0x1c];
    void*         m_Manager;
};

struct CGameScriptInterface
{
    unsigned char       m_Pad00[0x08];
    CGameScriptContext* m_Context;
};

static CThingOverlay* g_thing = 0;
static void* g_resolveA_in = 0;
static CGSIOnlyTarget_Component* g_observed_component = 0;
static void* g_observed_target = (void*)1;
static unsigned long g_set_calls = 0;
static CKeyPair* g_lower_bound_result = 0;
static ETCInterfaceType g_observed_key = ETC_DUMMY;
static CVectorMap* g_observed_map = 0;

extern "C" void* __fastcall Sub_ResolveA(void* self)
{
    g_resolveA_in = self;
    return (void*)0xAAAA;
}

extern "C" void* __fastcall Sub_ResolveB(void* obj)
{
    (void)obj;
    return g_thing;
}

const ETCInterfaceType kTargetType = (ETCInterfaceType)8;

CKeyPair* CVectorMap::LowerBound(const ETCInterfaceType* pKey)
{
    g_observed_map = this;
    g_observed_key = *pKey;
    return g_lower_bound_result;
}

void CGSIOnlyTarget_Component::SetOnlyTarget(void* pThing)
{
    g_observed_component = this;
    g_observed_target = pThing;
    ++g_set_calls;
}

void __fastcall CGameScriptInterface_ResetPlayerCreatureOnlyTarget(
    const CGameScriptInterface* self);

static void ResetObservations()
{
    g_resolveA_in = 0;
    g_observed_component = 0;
    g_observed_target = (void*)1;
    g_set_calls = 0;
    g_lower_bound_result = 0;
    g_observed_key = ETC_DUMMY;
    g_observed_map = 0;
}

static int Fail(const char* message)
{
    std::printf("CGameScriptInterface_00899e90_TEST FAIL: %s\n", message);
    return 1;
}

int main()
{
    CGameScriptInterface gsi = {};
    CGameScriptContext context = {};
    CThingOverlay thing = {};
    CGSIOnlyTarget_Component foundComponent;
    CGSIOnlyTarget_Component endComponent;
    CKeyPair foundEntry = {};
    CKeyPair greaterEntry = {};
    CKeyPair endEntry = {};

    gsi.m_Context = &context;
    context.m_Manager = (void*)0x1234;
    thing.m_InterfaceMap44.m_pEnd = &endEntry;
    foundEntry.m_Value = &foundComponent;
    endEntry.m_Value = &endComponent;

    ResetObservations();
    g_thing = 0;
    CGameScriptInterface_ResetPlayerCreatureOnlyTarget(&gsi);
    if (g_set_calls != 0 || g_resolveA_in != (void*)0x1234)
        return Fail("null player creature guard");

    ResetObservations();
    g_thing = &thing;
    thing.m_Flags91 = 1;
    thing.m_Flags20 = 0x100;
    CGameScriptInterface_ResetPlayerCreatureOnlyTarget(&gsi);
    if (g_set_calls != 0)
        return Fail("dead player creature guard");

    ResetObservations();
    thing.m_Flags91 = 0;
    thing.m_Flags20 = 0;
    CGameScriptInterface_ResetPlayerCreatureOnlyTarget(&gsi);
    if (g_set_calls != 0)
        return Fail("missing target capability");

    ResetObservations();
    thing.m_Flags20 = 0x100;
    foundEntry.m_Key = (ETCInterfaceType)8;
    g_lower_bound_result = &foundEntry;
    CGameScriptInterface_ResetPlayerCreatureOnlyTarget(&gsi);
    if (g_set_calls != 1 ||
        g_observed_component != &foundComponent ||
        g_observed_target != 0 ||
        g_observed_map != &thing.m_InterfaceMap44 ||
        (int)g_observed_key != 8)
        return Fail("exact component reset");

    ResetObservations();
    greaterEntry.m_Key = (ETCInterfaceType)9;
    greaterEntry.m_Value = &foundComponent;
    g_lower_bound_result = &greaterEntry;
    CGameScriptInterface_ResetPlayerCreatureOnlyTarget(&gsi);
    if (g_set_calls != 1 ||
        g_observed_component != &endComponent ||
        g_observed_target != 0)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_lower_bound_result = &endEntry;
    CGameScriptInterface_ResetPlayerCreatureOnlyTarget(&gsi);
    if (g_set_calls != 1 ||
        g_observed_component != &endComponent ||
        g_observed_target != 0)
        return Fail("direct sentinel");

    std::printf("CGameScriptInterface_00899e90_TEST PASS\n");
    return 0;
}
