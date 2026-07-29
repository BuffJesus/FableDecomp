#include <cstdio>

enum ETCInterfaceType { ETC_DUMMY = 0 };

class CGSIPicklockMoralityStats
{
public:
    void SetMorality(long morality);

    unsigned char m_Pad00[0x28];
    long          m_Morality;
};

struct CKeyPair
{
    ETCInterfaceType           m_Key;
    CGSIPicklockMoralityStats* m_Value;
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

struct CPicklockMoralityDefinition
{
    unsigned char m_Pad00[0xe8];
    long          m_Change;
};

struct CDefinitionManagerOverlay
{
    unsigned char                m_Pad00[0xdc];
    CPicklockMoralityDefinition* m_Definition;
};

struct CGameScriptInterface
{
    unsigned char m_Pad00[0x14];
    void*         m_Field14;
};

static CThingOverlay* g_thing = 0;
static void* g_resolve_a_input = 0;
static CDefinitionManagerOverlay g_definition_manager = {};
static CPicklockMoralityDefinition g_definition = {};
static unsigned long g_definition_calls = 0;
static CGSIPicklockMoralityStats* g_set_stats = 0;
static long g_set_value = 0;
static unsigned long g_set_calls = 0;
static CKeyPair* g_lower_bound_result = 0;
static CVectorMap* g_observed_map = 0;
static ETCInterfaceType g_observed_key = ETC_DUMMY;

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

extern "C" CDefinitionManagerOverlay* __cdecl Sub_GetDefinitionManager()
{
    ++g_definition_calls;
    return &g_definition_manager;
}

const ETCInterfaceType kTargetType = (ETCInterfaceType)4;

CKeyPair* CVectorMap::LowerBound(const ETCInterfaceType* pKey)
{
    g_observed_map = this;
    g_observed_key = *pKey;
    return g_lower_bound_result;
}

void CGSIPicklockMoralityStats::SetMorality(long morality)
{
    g_set_stats = this;
    g_set_value = morality;
    ++g_set_calls;
    m_Morality = morality;
}

void __fastcall CGameScriptInterface_ChangeHeroMoralityDueToPicklock(
    const CGameScriptInterface* self);

static void ResetObservations()
{
    g_resolve_a_input = 0;
    g_definition_calls = 0;
    g_set_stats = 0;
    g_set_value = 0;
    g_set_calls = 0;
    g_lower_bound_result = 0;
    g_observed_map = 0;
    g_observed_key = ETC_DUMMY;
}

static int Fail(const char* message)
{
    std::printf("CGameScriptInterface_0089a0e0_TEST FAIL: %s\n", message);
    return 1;
}

int main()
{
    CGameScriptInterface gsi = {};
    CThingOverlay hero = {};
    CGSIPicklockMoralityStats foundStats = {};
    CGSIPicklockMoralityStats endStats = {};
    CKeyPair foundEntry = {};
    CKeyPair greaterEntry = {};
    CKeyPair endEntry = {};

    gsi.m_Field14 = (void*)0x1234;
    g_definition_manager.m_Definition = &g_definition;
    hero.m_InterfaceMap44.m_pEnd = &endEntry;
    foundEntry.m_Value = &foundStats;
    endEntry.m_Value = &endStats;

    ResetObservations();
    g_thing = 0;
    CGameScriptInterface_ChangeHeroMoralityDueToPicklock(&gsi);
    if (g_set_calls != 0 ||
        g_definition_calls != 0 ||
        g_resolve_a_input != (void*)0x1234)
        return Fail("null hero guard");

    ResetObservations();
    g_thing = &hero;
    hero.m_Flags91 = 1;
    hero.m_Flags20 = 0x10;
    CGameScriptInterface_ChangeHeroMoralityDueToPicklock(&gsi);
    if (g_set_calls != 0 || g_definition_calls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    hero.m_Flags20 = 0;
    CGameScriptInterface_ChangeHeroMoralityDueToPicklock(&gsi);
    if (g_set_calls != 0 || g_definition_calls != 0)
        return Fail("missing HeroStats capability");

    ResetObservations();
    hero.m_Flags20 = 0x10;
    foundEntry.m_Key = (ETCInterfaceType)4;
    foundStats.m_Morality = 25;
    g_definition.m_Change = -4;
    g_lower_bound_result = &foundEntry;
    CGameScriptInterface_ChangeHeroMoralityDueToPicklock(&gsi);
    if (g_set_calls != 1 ||
        g_set_stats != &foundStats ||
        g_set_value != 21 ||
        g_definition_calls != 1 ||
        g_observed_map != &hero.m_InterfaceMap44 ||
        (int)g_observed_key != 4)
        return Fail("exact HeroStats picklock change");

    ResetObservations();
    greaterEntry.m_Key = (ETCInterfaceType)5;
    greaterEntry.m_Value = &foundStats;
    endStats.m_Morality = 8;
    g_definition.m_Change = -6;
    g_lower_bound_result = &greaterEntry;
    CGameScriptInterface_ChangeHeroMoralityDueToPicklock(&gsi);
    if (g_set_stats != &endStats || g_set_value != 2)
        return Fail("greater-key sentinel");

    ResetObservations();
    endStats.m_Morality = -3;
    g_definition.m_Change = 7;
    g_lower_bound_result = &endEntry;
    CGameScriptInterface_ChangeHeroMoralityDueToPicklock(&gsi);
    if (g_set_stats != &endStats || g_set_value != 4)
        return Fail("direct sentinel");

    std::printf("CGameScriptInterface_0089a0e0_TEST PASS\n");
    return 0;
}
