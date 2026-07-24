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

// ---- test doubles for the externs ----
static CThingOverlay* g_thing = 0;
static void* g_resolveA_in = 0;
static void* g_target_iface = 0;
static long  g_target_ret = 0;

void* Sub_ResolveA(void* self) { g_resolveA_in = self; return (void*)0xAAAA; }
void* Sub_ResolveB(void* obj)  { (void)obj; return g_thing; }
long  Sub_InvokeTarget(void* iface) { g_target_iface = iface; return g_target_ret; }

const ETCInterfaceType kTargetType = (ETCInterfaceType)0x68;

// map storage
static CKeyPair g_entries[4];

CKeyPair* CVectorMap::LowerBound(const ETCInterfaceType* pKey)
{
    for (CKeyPair* p = m_pBegin; p != m_pEnd; ++p)
        if ((int)p->m_Key >= (int)*pKey)
            return p;
    return m_pEnd;
}

long CGameScriptInterface_GetPlayerCreatureCombatMultiplierRunningNumHits(
    const CGameScriptInterface* self)
{
    void* a = Sub_ResolveA(self->m_Field14);
    CThingOverlay* pThing = (CThingOverlay*)Sub_ResolveB(a);
    if (pThing == 0)
        return 0;
    if ((pThing->m_Flags91 & 0x01) != 0)
        return 0;
    if ((pThing->m_Flags2C & 0x100) == 0)
        return 0;

    ETCInterfaceType key = kTargetType;
    CKeyPair* pEntry = pThing->m_InterfaceMap44.LowerBound(&key);
    CKeyPair* pEnd = pThing->m_InterfaceMap44.m_pEnd;
    if (pEntry == pEnd || (int)pEntry->m_Key > (int)key)
        pEntry = pEnd;

    return Sub_InvokeTarget(pEntry->m_Value);
}

int main()
{
    CGameScriptInterface gsi;
    for (int i = 0; i < (int)sizeof(gsi); ++i) ((unsigned char*)&gsi)[i] = 0;
    gsi.m_Field14 = (void*)0x1234;

    // Case 1: null thing -> return 0
    g_thing = 0;
    if (CGameScriptInterface_GetPlayerCreatureCombatMultiplierRunningNumHits(&gsi) != 0)
    { std::printf("FAIL null\n"); return 1; }

    // set up a valid thing
    static CThingOverlay thing;
    for (int i = 0; i < (int)sizeof(thing); ++i) ((unsigned char*)&thing)[i] = 0;
    g_thing = &thing;

    // Case 2: flag91 bit set -> 0
    thing.m_Flags91 = 0x01;
    thing.m_Flags2C = 0x100;
    if (CGameScriptInterface_GetPlayerCreatureCombatMultiplierRunningNumHits(&gsi) != 0)
    { std::printf("FAIL flag91\n"); return 1; }

    // Case 3: flag2C bit clear -> 0
    thing.m_Flags91 = 0x00;
    thing.m_Flags2C = 0x000;
    if (CGameScriptInterface_GetPlayerCreatureCombatMultiplierRunningNumHits(&gsi) != 0)
    { std::printf("FAIL flag2C\n"); return 1; }

    // Case 4: valid path, entry with key 0x68 present -> invoke its value
    thing.m_Flags91 = 0x00;
    thing.m_Flags2C = 0x100;
    g_entries[0].m_Key = (ETCInterfaceType)0x10; g_entries[0].m_Value = (void*)0x11;
    g_entries[1].m_Key = (ETCInterfaceType)0x68; g_entries[1].m_Value = (void*)0x68680000;
    g_entries[2].m_Key = (ETCInterfaceType)0x90; g_entries[2].m_Value = (void*)0x99;
    thing.m_InterfaceMap44.m_pBegin = &g_entries[0];
    thing.m_InterfaceMap44.m_pEnd   = &g_entries[3];
    g_target_ret = 4242;
    long r = CGameScriptInterface_GetPlayerCreatureCombatMultiplierRunningNumHits(&gsi);
    if (r != 4242 || g_target_iface != (void*)0x68680000)
    { std::printf("FAIL found path r=%ld iface=%p\n", r, g_target_iface); return 1; }

    // Case 5: key 0x68 absent (next key > 0x68) -> uses end entry value
    g_entries[1].m_Key = (ETCInterfaceType)0x70; g_entries[1].m_Value = (void*)0x70;
    // end sentinel value: set m_pEnd's m_Value via g_entries[3]? m_pEnd points at g_entries[3]
    g_entries[3].m_Key = (ETCInterfaceType)0x0; g_entries[3].m_Value = (void*)0xEEEE;
    g_target_iface = 0;
    r = CGameScriptInterface_GetPlayerCreatureCombatMultiplierRunningNumHits(&gsi);
    if (g_target_iface != (void*)0xEEEE)
    { std::printf("FAIL absent path iface=%p\n", g_target_iface); return 1; }

    std::printf("CGameScriptInterface_00899cf0_TEST PASS\n");
    return 0;
}