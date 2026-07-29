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
static void* g_target_iface = 0;
static long  g_target_ret = 0;

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

extern "C" long __fastcall Sub_GetExperiencePickUpMultiplier(void* iface)
{
    g_target_iface = iface;
    return g_target_ret;
}

const ETCInterfaceType kTargetType = (ETCInterfaceType)0x68;

static CKeyPair g_entries[4];

CKeyPair* CVectorMap::LowerBound(const ETCInterfaceType* pKey)
{
    for (CKeyPair* p = m_pBegin; p != m_pEnd; ++p)
        if ((int)p->m_Key >= (int)*pKey)
            return p;
    return m_pEnd;
}

long __fastcall CGameScriptInterface_GetPlayerCreatureCombatMultiplier(
    const CGameScriptInterface* self);

int main()
{
    CGameScriptInterface gsi;
    for (int i = 0; i < (int)sizeof(gsi); ++i)
        ((unsigned char*)&gsi)[i] = 0;
    gsi.m_Field14 = (void*)0x1234;

    g_thing = 0;
    if (CGameScriptInterface_GetPlayerCreatureCombatMultiplier(&gsi) != 1)
    { std::printf("FAIL null\n"); return 1; }

    static CThingOverlay thing;
    for (int i = 0; i < (int)sizeof(thing); ++i)
        ((unsigned char*)&thing)[i] = 0;
    g_thing = &thing;

    thing.m_Flags91 = 0x01;
    thing.m_Flags2C = 0x100;
    if (CGameScriptInterface_GetPlayerCreatureCombatMultiplier(&gsi) != 1)
    { std::printf("FAIL dead\n"); return 1; }

    thing.m_Flags91 = 0x00;
    thing.m_Flags2C = 0;
    if (CGameScriptInterface_GetPlayerCreatureCombatMultiplier(&gsi) != 1)
    { std::printf("FAIL capability\n"); return 1; }

    thing.m_Flags2C = 0x100;
    g_entries[0].m_Key = (ETCInterfaceType)0x10;
    g_entries[0].m_Value = (void*)0x11;
    g_entries[1].m_Key = (ETCInterfaceType)0x68;
    g_entries[1].m_Value = (void*)0x68680000;
    g_entries[2].m_Key = (ETCInterfaceType)0x90;
    g_entries[2].m_Value = (void*)0x99;
    thing.m_InterfaceMap44.m_pBegin = &g_entries[0];
    thing.m_InterfaceMap44.m_pEnd = &g_entries[3];
    g_target_ret = 37;

    long r = CGameScriptInterface_GetPlayerCreatureCombatMultiplier(&gsi);
    if (r != 37 || g_target_iface != (void*)0x68680000 ||
        g_resolveA_in != (void*)0x1234)
    { std::printf("FAIL found r=%ld iface=%p\n", r, g_target_iface); return 1; }

    g_entries[1].m_Key = (ETCInterfaceType)0x70;
    g_entries[1].m_Value = (void*)0x70;
    g_entries[3].m_Key = (ETCInterfaceType)0;
    g_entries[3].m_Value = (void*)0xEEEE;
    g_target_iface = 0;
    g_target_ret = 41;
    r = CGameScriptInterface_GetPlayerCreatureCombatMultiplier(&gsi);
    if (r != 41 || g_target_iface != (void*)0xEEEE)
    { std::printf("FAIL sentinel r=%ld iface=%p\n", r, g_target_iface); return 1; }

    std::printf("CGameScriptInterface_00899c90_TEST PASS\n");
    return 0;
}
