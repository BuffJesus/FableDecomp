#include <cstdio>

enum ETCInterfaceType { ETC_DUMMY = 0 };

class CGSIMoralityStats
{
public:
    long GetMoralityMax();
    void SetMorality(long morality);

    unsigned char m_Pad00[0x28];
    long          m_Morality;
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

static CThingOverlay* g_first_thing = 0;
static CThingOverlay* g_second_thing = 0;
static unsigned long g_resolve_a_calls = 0;
static unsigned long g_resolve_b_calls = 0;
static void* g_resolve_a_input = 0;
static long g_maximum = 100;
static CGSIMoralityStats* g_get_stats = 0;
static CGSIMoralityStats* g_set_stats = 0;
static long g_set_value = 0;
static unsigned long g_get_calls = 0;
static unsigned long g_set_calls = 0;
static CKeyPair* g_lower_bound_result = 0;
static CVectorMap* g_observed_map = 0;
static ETCInterfaceType g_observed_key = ETC_DUMMY;

extern "C" void* __fastcall Sub_ResolveA(void* self)
{
    g_resolve_a_input = self;
    ++g_resolve_a_calls;
    return (void*)0xAAAA;
}

extern "C" CThingOverlay* __fastcall Sub_ResolveB(void* obj)
{
    (void)obj;
    CThingOverlay* result =
        g_resolve_b_calls == 0 ? g_first_thing : g_second_thing;
    ++g_resolve_b_calls;
    return result;
}

const ETCInterfaceType kTargetType = (ETCInterfaceType)4;

CKeyPair* CVectorMap::LowerBound(const ETCInterfaceType* pKey)
{
    g_observed_map = this;
    g_observed_key = *pKey;
    return g_lower_bound_result;
}

long CGSIMoralityStats::GetMoralityMax()
{
    g_get_stats = this;
    ++g_get_calls;
    return g_maximum;
}

void CGSIMoralityStats::SetMorality(long morality)
{
    g_set_stats = this;
    g_set_value = morality;
    ++g_set_calls;
    m_Morality = morality;
}

void __fastcall CGameScriptInterface_GiveHeroMorality(
    const CGameScriptInterface* self,
    float amount);

static void ResetObservations()
{
    g_resolve_a_calls = 0;
    g_resolve_b_calls = 0;
    g_resolve_a_input = 0;
    g_get_stats = 0;
    g_set_stats = 0;
    g_set_value = 0;
    g_get_calls = 0;
    g_set_calls = 0;
    g_lower_bound_result = 0;
    g_observed_map = 0;
    g_observed_key = ETC_DUMMY;
}

static int Fail(const char* message)
{
    std::printf("CGameScriptInterface_00899ef0_TEST FAIL: %s\n", message);
    return 1;
}

int main()
{
    CGameScriptInterface gsi = {};
    CThingOverlay hero = {};
    CGSIMoralityStats foundStats = {};
    CGSIMoralityStats endStats = {};
    CKeyPair foundEntry = {};
    CKeyPair greaterEntry = {};
    CKeyPair endEntry = {};

    gsi.m_Field14 = (void*)0x1234;
    g_first_thing = &hero;
    g_second_thing = &hero;
    hero.m_InterfaceMap44.m_pEnd = &endEntry;
    foundEntry.m_Value = &foundStats;
    endEntry.m_Value = &endStats;

    ResetObservations();
    g_first_thing = 0;
    CGameScriptInterface_GiveHeroMorality(&gsi, 0.25f);
    if (g_resolve_a_calls != 1 ||
        g_resolve_b_calls != 1 ||
        g_get_calls != 0 ||
        g_set_calls != 0 ||
        g_resolve_a_input != (void*)0x1234)
        return Fail("null hero guard");

    ResetObservations();
    g_first_thing = &hero;
    hero.m_Flags91 = 1;
    CGameScriptInterface_GiveHeroMorality(&gsi, 0.25f);
    if (g_resolve_a_calls != 1 ||
        g_resolve_b_calls != 1 ||
        g_get_calls != 0 ||
        g_set_calls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    union PointerFloat
    {
        CGSIMoralityStats* pPointer;
        float fBits;
    } fallback;
    foundStats.m_Morality = 10;
    fallback.pPointer = &foundStats;
    g_maximum = 100;
    CGameScriptInterface_GiveHeroMorality(&gsi, fallback.fBits);
    if (g_resolve_a_calls != 2 ||
        g_resolve_b_calls != 2 ||
        g_get_stats != &foundStats ||
        g_set_stats != &foundStats ||
        g_set_value != 10 ||
        g_observed_map != 0)
        return Fail("legacy amount-bits fallback");

    ResetObservations();
    hero.m_Flags20 = 0x10;
    foundEntry.m_Key = (ETCInterfaceType)4;
    foundStats.m_Morality = 20;
    g_lower_bound_result = &foundEntry;
    g_maximum = 80;
    CGameScriptInterface_GiveHeroMorality(&gsi, -0.25f);
    if (g_get_stats != &foundStats ||
        g_set_stats != &foundStats ||
        g_set_value != 0 ||
        g_observed_map != &hero.m_InterfaceMap44 ||
        (int)g_observed_key != 4)
        return Fail("exact HeroStats component");

    ResetObservations();
    greaterEntry.m_Key = (ETCInterfaceType)5;
    greaterEntry.m_Value = &foundStats;
    endStats.m_Morality = 7;
    g_lower_bound_result = &greaterEntry;
    g_maximum = 40;
    CGameScriptInterface_GiveHeroMorality(&gsi, 0.5f);
    if (g_set_stats != &endStats || g_set_value != 27)
        return Fail("greater-key sentinel");

    ResetObservations();
    endStats.m_Morality = 1;
    g_lower_bound_result = &endEntry;
    g_maximum = 20;
    CGameScriptInterface_GiveHeroMorality(&gsi, 0.5f);
    if (g_set_stats != &endStats || g_set_value != 11)
        return Fail("direct sentinel");

    std::printf("CGameScriptInterface_00899ef0_TEST PASS\n");
    return 0;
}
