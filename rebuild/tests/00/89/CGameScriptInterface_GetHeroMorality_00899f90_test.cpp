#include <cstdio>

enum ETCInterfaceType { ETC_DUMMY = 0 };

class CGSIMoralityStats
{
public:
    float GetMoralityPercentage();
};

struct CKeyPair
{
    ETCInterfaceType   m_Key;
    CGSIMoralityStats* m_Value;
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
static CGSIMoralityStats* g_observed_stats = 0;
static float g_percentage = 0.0f;
static unsigned long g_percentage_calls = 0;
static CKeyPair* g_lower_bound_result = 0;
static CVectorMap* g_observed_map = 0;
static ETCInterfaceType g_observed_key = ETC_DUMMY;

extern "C" float g_DefaultMorality = -0.75f;

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
    return g_lower_bound_result;
}

float CGSIMoralityStats::GetMoralityPercentage()
{
    g_observed_stats = this;
    ++g_percentage_calls;
    return g_percentage;
}

float __fastcall CGameScriptInterface_GetHeroMorality(
    const CGameScriptInterface* self);

static void ResetObservations()
{
    g_resolve_a_input = 0;
    g_observed_stats = 0;
    g_percentage = 0.0f;
    g_percentage_calls = 0;
    g_lower_bound_result = 0;
    g_observed_map = 0;
    g_observed_key = ETC_DUMMY;
}

static int Fail(const char* message)
{
    std::printf("CGameScriptInterface_00899f90_TEST FAIL: %s\n", message);
    return 1;
}

int main()
{
    CGameScriptInterface gsi = {};
    CThingOverlay hero = {};
    CGSIMoralityStats foundStats;
    CGSIMoralityStats endStats;
    CKeyPair foundEntry = {};
    CKeyPair greaterEntry = {};
    CKeyPair endEntry = {};

    gsi.m_Field14 = (void*)0x1234;
    hero.m_InterfaceMap44.m_pEnd = &endEntry;
    foundEntry.m_Value = &foundStats;
    endEntry.m_Value = &endStats;

    ResetObservations();
    g_thing = 0;
    if (CGameScriptInterface_GetHeroMorality(&gsi) != -0.75f ||
        g_percentage_calls != 0 ||
        g_resolve_a_input != (void*)0x1234)
        return Fail("null hero default");

    ResetObservations();
    g_thing = &hero;
    hero.m_Flags91 = 1;
    if (CGameScriptInterface_GetHeroMorality(&gsi) != -0.75f ||
        g_percentage_calls != 0)
        return Fail("dead hero default");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    g_percentage = 0.125f;
    if (CGameScriptInterface_GetHeroMorality(&gsi) != 0.125f ||
        g_percentage_calls != 1 ||
        g_observed_stats !=
            reinterpret_cast<CGSIMoralityStats*>(&gsi) ||
        g_observed_map != 0)
        return Fail("legacy self fallback");

    ResetObservations();
    hero.m_Flags20 = 0x10;
    foundEntry.m_Key = (ETCInterfaceType)4;
    g_lower_bound_result = &foundEntry;
    g_percentage = 0.625f;
    if (CGameScriptInterface_GetHeroMorality(&gsi) != 0.625f ||
        g_percentage_calls != 1 ||
        g_observed_stats != &foundStats ||
        g_observed_map != &hero.m_InterfaceMap44 ||
        (int)g_observed_key != 4)
        return Fail("exact HeroStats component");

    ResetObservations();
    greaterEntry.m_Key = (ETCInterfaceType)5;
    greaterEntry.m_Value = &foundStats;
    g_lower_bound_result = &greaterEntry;
    g_percentage = -0.25f;
    if (CGameScriptInterface_GetHeroMorality(&gsi) != -0.25f ||
        g_observed_stats != &endStats)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_lower_bound_result = &endEntry;
    g_percentage = 1.0f;
    if (CGameScriptInterface_GetHeroMorality(&gsi) != 1.0f ||
        g_observed_stats != &endStats)
        return Fail("direct sentinel");

    std::printf("CGameScriptInterface_00899f90_TEST PASS\n");
    return 0;
}
