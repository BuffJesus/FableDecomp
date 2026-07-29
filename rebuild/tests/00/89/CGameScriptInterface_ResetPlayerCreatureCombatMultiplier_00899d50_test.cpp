#include <cstdio>

enum ETCInterfaceType { ETC_DUMMY = 0 };

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
    unsigned char m_Pad00[0x2c];
    unsigned int  m_Flags2C;
    unsigned char m_Pad30[0x14];
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
static void* g_resolveA_in = 0;
static void* g_reset_iface = 0;
static unsigned long g_reset_calls = 0;
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

extern "C" void __fastcall Sub_ResetExperiencePickUpMultiplier(void* iface)
{
    g_reset_iface = iface;
    ++g_reset_calls;
}

const ETCInterfaceType kTargetType = (ETCInterfaceType)0x68;

CKeyPair* CVectorMap::LowerBound(const ETCInterfaceType* pKey)
{
    g_observed_map = this;
    g_observed_key = *pKey;
    return g_lower_bound_result;
}

void __fastcall CGameScriptInterface_ResetPlayerCreatureCombatMultiplier(
    const CGameScriptInterface* self);

static void ResetObservations()
{
    g_resolveA_in = 0;
    g_reset_iface = 0;
    g_reset_calls = 0;
    g_lower_bound_result = 0;
    g_observed_key = ETC_DUMMY;
    g_observed_map = 0;
}

static int Fail(const char* message)
{
    std::printf("CGameScriptInterface_00899d50_TEST FAIL: %s\n", message);
    return 1;
}

int main()
{
    CGameScriptInterface gsi = {};
    CThingOverlay thing = {};
    CKeyPair foundEntry = {};
    CKeyPair greaterEntry = {};
    CKeyPair endEntry = {};

    gsi.m_Field14 = (void*)0x1234;
    thing.m_InterfaceMap44.m_pEnd = &endEntry;

    ResetObservations();
    g_thing = 0;
    CGameScriptInterface_ResetPlayerCreatureCombatMultiplier(&gsi);
    if (g_reset_calls != 0 || g_resolveA_in != (void*)0x1234)
        return Fail("null hero guard");

    ResetObservations();
    g_thing = &thing;
    thing.m_Flags91 = 1;
    thing.m_Flags2C = 0x100;
    CGameScriptInterface_ResetPlayerCreatureCombatMultiplier(&gsi);
    if (g_reset_calls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    thing.m_Flags91 = 0;
    thing.m_Flags2C = 0;
    CGameScriptInterface_ResetPlayerCreatureCombatMultiplier(&gsi);
    if (g_reset_calls != 0)
        return Fail("missing capability guard");

    ResetObservations();
    thing.m_Flags2C = 0x100;
    foundEntry.m_Key = (ETCInterfaceType)0x68;
    foundEntry.m_Value = (void*)0x68680000;
    g_lower_bound_result = &foundEntry;
    CGameScriptInterface_ResetPlayerCreatureCombatMultiplier(&gsi);
    if (g_reset_calls != 1 ||
        g_reset_iface != (void*)0x68680000 ||
        g_observed_map != &thing.m_InterfaceMap44 ||
        (int)g_observed_key != 0x68)
        return Fail("exact component reset");

    ResetObservations();
    greaterEntry.m_Key = (ETCInterfaceType)0x70;
    greaterEntry.m_Value = (void*)0x70;
    endEntry.m_Value = (void*)0xEEEE;
    g_lower_bound_result = &greaterEntry;
    CGameScriptInterface_ResetPlayerCreatureCombatMultiplier(&gsi);
    if (g_reset_calls != 1 || g_reset_iface != (void*)0xEEEE)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_lower_bound_result = &endEntry;
    CGameScriptInterface_ResetPlayerCreatureCombatMultiplier(&gsi);
    if (g_reset_calls != 1 || g_reset_iface != (void*)0xEEEE)
        return Fail("direct sentinel");

    std::printf("CGameScriptInterface_00899d50_TEST PASS\n");
    return 0;
}
