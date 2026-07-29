#include <cstdio>

enum ETCInterfaceType { ETC_DUMMY = 0 };

struct CGSIWillEnergyMaxComponent
{
    unsigned char m_Pad00[0x5c];
    long          m_MaximumEnergy;
};

struct CKeyPair
{
    ETCInterfaceType m_Key;
    void*            m_Value;
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
    unsigned char m_Flags20;
    unsigned char m_Pad21[0x23];
    CVectorMap    m_InterfaceMap44;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

struct CGameScriptInterface
{
    unsigned char m_Pad00[0x14];
    void*         m_Field14;
};

static CThingOverlay* g_thing = 0;
static void* g_resolve_a_input = 0;
static CKeyPair* g_lower_bound_result = 0;
static CVectorMap* g_observed_map = 0;
static ETCInterfaceType g_observed_key = ETC_DUMMY;
static unsigned long g_lower_bound_calls = 0;

extern "C" void* __fastcall Sub_ResolveA(void* self)
{
    g_resolve_a_input = self;
    return (void*)0xAAAA;
}

extern "C" CThingOverlay* __fastcall Sub_ResolveB(void* obj)
{
    (void)obj;
    return g_thing;
}

const ETCInterfaceType kTargetType = (ETCInterfaceType)4;

CKeyPair* CVectorMap::LowerBound(const ETCInterfaceType* pKey)
{
    g_observed_map = this;
    g_observed_key = *pKey;
    ++g_lower_bound_calls;
    return g_lower_bound_result;
}

long __fastcall CGameScriptInterface_GetHeroWillEnergyMax(
    const CGameScriptInterface* self);

static void ResetObservations()
{
    g_resolve_a_input = 0;
    g_lower_bound_result = 0;
    g_observed_map = 0;
    g_observed_key = ETC_DUMMY;
    g_lower_bound_calls = 0;
}

static int Fail(const char* message)
{
    std::printf("CGameScriptInterface_0089a1b0_TEST FAIL: %s\n", message);
    return 1;
}

int main()
{
    CGameScriptInterface gsi = {};
    CThingOverlay hero = {};
    CGSIWillEnergyMaxComponent foundComponent = {};
    CGSIWillEnergyMaxComponent endComponent = {};
    CKeyPair foundEntry = {};
    CKeyPair greaterEntry = {};
    CKeyPair endEntry = {};

    gsi.m_Field14 = (void*)0x1234;
    hero.m_InterfaceMap44.m_pEnd = &endEntry;
    foundEntry.m_Value = &foundComponent;
    endEntry.m_Value = &endComponent;

    ResetObservations();
    g_thing = 0;
    if (CGameScriptInterface_GetHeroWillEnergyMax(&gsi) != 0 ||
        g_lower_bound_calls != 0 ||
        g_resolve_a_input != (void*)0x1234)
        return Fail("null hero guard");

    ResetObservations();
    g_thing = &hero;
    hero.m_Flags91 = 1;
    hero.m_Flags20 = 0x10;
    if (CGameScriptInterface_GetHeroWillEnergyMax(&gsi) != 0 ||
        g_lower_bound_calls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    if (CGameScriptInterface_GetHeroWillEnergyMax(&gsi) != 0 ||
        g_lower_bound_calls != 0)
        return Fail("missing HeroStats capability");

    ResetObservations();
    hero.m_Flags20 = 0x10;
    foundEntry.m_Key = (ETCInterfaceType)4;
    foundComponent.m_MaximumEnergy = 120;
    g_lower_bound_result = &foundEntry;
    if (CGameScriptInterface_GetHeroWillEnergyMax(&gsi) != 120 ||
        g_observed_map != &hero.m_InterfaceMap44 ||
        (int)g_observed_key != 4 ||
        g_lower_bound_calls != 1)
        return Fail("exact HeroStats maximum");

    ResetObservations();
    greaterEntry.m_Key = (ETCInterfaceType)5;
    greaterEntry.m_Value = &foundComponent;
    endComponent.m_MaximumEnergy = 88;
    g_lower_bound_result = &greaterEntry;
    if (CGameScriptInterface_GetHeroWillEnergyMax(&gsi) != 88)
        return Fail("greater-key sentinel");

    ResetObservations();
    endComponent.m_MaximumEnergy = -9;
    g_lower_bound_result = &endEntry;
    if (CGameScriptInterface_GetHeroWillEnergyMax(&gsi) != -9)
        return Fail("direct sentinel");

    std::printf("CGameScriptInterface_0089a1b0_TEST PASS\n");
    return 0;
}
