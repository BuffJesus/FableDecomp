/* Standalone behavioural test for
 * CGameScriptInterface::IsPlayerCreatureReadyToFireProjectileWeapon @ 0x00899c10
 * Self-contained: defines its own copy of the fn + recording stubs for externs.
 * Prints exactly "FSE2_00899c10_TEST PASS" on success; returns nonzero on any fail.
 */
#include <cstdio>

/* ---- overlays (mirror the source) ---- */
struct IsPCReady_Self
{
    char   pad00[0x14];
    void*  m_Field14;
};

struct IsPCReady_Creature
{
    char           pad00[0x30];
    unsigned char  m_Flags30;
    char           pad31[0x91 - 0x31];
    unsigned char  m_Flags91;
};

struct IsPCReady_MapNode
{
    int    m_Key;
    void*  m_Value;
};

struct IsPCReady_Map
{
    IsPCReady_MapNode* m_Begin;
    IsPCReady_MapNode* m_End;
    IsPCReady_MapNode* LowerBound(int* key);
};

/* ---- recording stubs for the externs ---- */
static void*             g_subsystemIn;
static void*             g_creatureRet;
static void*             g_lowerBoundThis;
static int               g_lowerBoundKey;
static IsPCReady_MapNode* g_lowerBoundRet;
static void*             g_readinessArg;
static float             g_readinessRet;

const float IsPCReady_ReadyThreshold = 0.5f;

void* __fastcall IsPCReady_GetSubsystem(void* thisField14)
{
    g_subsystemIn = thisField14;
    return (void*)0xA11CE;   /* opaque subsystem handle */
}

static void* g_getCreatureArg;
void* __fastcall IsPCReady_GetCreature(void* subsystem)
{
    g_getCreatureArg = subsystem;
    return g_creatureRet;
}

/* __fastcall member -> this in ecx, &key pushed on stack */
IsPCReady_MapNode* IsPCReady_Map::LowerBound(int* key)
{
    g_lowerBoundThis = this;
    g_lowerBoundKey  = *key;
    return g_lowerBoundRet;
}

float __fastcall IsPCReady_GetReadiness(void* weapon)
{
    g_readinessArg = weapon;
    return g_readinessRet;
}

/* ---- the function under test (copy of source) ---- */
bool __fastcall CGameScriptInterface_IsPlayerCreatureReadyToFireProjectileWeapon(
    void* thisptr, unsigned long /*edx*/, float* out)
{
    IsPCReady_Self* self = (IsPCReady_Self*)thisptr;

    void* subsystem = IsPCReady_GetSubsystem(self->m_Field14);
    IsPCReady_Creature* creature = (IsPCReady_Creature*)IsPCReady_GetCreature(subsystem);

    if (creature == 0)
        return false;
    if (creature->m_Flags91 & 0x1)
        return false;
    if (!(creature->m_Flags30 & 0x2))
        return false;

    IsPCReady_Map* map = (IsPCReady_Map*)((char*)creature + 0x44);

    int key = 0x81;
    IsPCReady_MapNode* node = map->LowerBound(&key);
    IsPCReady_MapNode* end = map->m_End;

    if (node != end && node->m_Key <= 0x81)
    {
    }
    else
    {
        node = end;
    }

    float readiness = IsPCReady_GetReadiness(node->m_Value);

    if (readiness > IsPCReady_ReadyThreshold)
    {
        *out = readiness;
        return true;
    }
    return false;
}

/* ---- driver ---- */
static IsPCReady_Creature* make_creature(unsigned char f30, unsigned char f91,
                                         IsPCReady_MapNode* end)
{
    static char buf[0x100];
    for (int i = 0; i < 0x100; ++i) buf[i] = 0;
    IsPCReady_Creature* c = (IsPCReady_Creature*)buf;
    c->m_Flags30 = f30;
    c->m_Flags91 = f91;
    /* map lives at +0x44: begin @+0x44, end @+0x48 */
    IsPCReady_Map* m = (IsPCReady_Map*)(buf + 0x44);
    m->m_Begin = 0;
    m->m_End   = end;
    return c;
}

int main(void)
{
    IsPCReady_Self self;
    self.m_Field14 = (void*)0x1234;

    float out;

    /* ---- case 1: null creature -> false ---- */
    g_creatureRet = 0;
    if (CGameScriptInterface_IsPlayerCreatureReadyToFireProjectileWeapon(&self, 0, &out))
        { printf("FAIL null-creature\n"); return 1; }
    if (g_subsystemIn != (void*)0x1234) { printf("FAIL field14 passthrough\n"); return 1; }
    if (g_getCreatureArg != (void*)0xA11CE) { printf("FAIL subsystem passthrough\n"); return 1; }

    /* ---- case 2: flag91 bit0 set -> false ---- */
    g_creatureRet = make_creature(0x02, 0x01, 0);
    if (CGameScriptInterface_IsPlayerCreatureReadyToFireProjectileWeapon(&self, 0, &out))
        { printf("FAIL flag91\n"); return 1; }

    /* ---- case 3: flag30 bit1 clear -> false ---- */
    g_creatureRet = make_creature(0x00, 0x00, 0);
    if (CGameScriptInterface_IsPlayerCreatureReadyToFireProjectileWeapon(&self, 0, &out))
        { printf("FAIL flag30\n"); return 1; }

    /* ---- case 4: found node, readiness > threshold -> true, out written ---- */
    {
        IsPCReady_MapNode found; found.m_Key = 0x50; found.m_Value = (void*)0xBEEF;
        IsPCReady_MapNode endnode; endnode.m_Key = 0; endnode.m_Value = 0;
        IsPCReady_Creature* c = make_creature(0x02, 0x00, &endnode);
        g_creatureRet   = c;
        g_lowerBoundRet = &found;
        g_readinessRet  = 0.9f;
        out = -1.0f;
        bool r = CGameScriptInterface_IsPlayerCreatureReadyToFireProjectileWeapon(&self, 0, &out);
        if (!r) { printf("FAIL ready true\n"); return 1; }
        if (out != 0.9f) { printf("FAIL out written\n"); return 1; }
        if (g_lowerBoundThis != (void*)((char*)c + 0x44)) { printf("FAIL map this\n"); return 1; }
        if (g_lowerBoundKey != 0x81) { printf("FAIL key 0x81\n"); return 1; }
        if (g_readinessArg != (void*)0xBEEF) { printf("FAIL weapon value\n"); return 1; }
    }

    /* ---- case 5: found node key > 0x81 -> use end sentinel value ---- */
    {
        IsPCReady_MapNode found; found.m_Key = 0x90; found.m_Value = (void*)0x1;
        IsPCReady_MapNode endnode; endnode.m_Key = 0; endnode.m_Value = (void*)0xEEEE;
        IsPCReady_Creature* c = make_creature(0x02, 0x00, &endnode);
        g_creatureRet   = c;
        g_lowerBoundRet = &found;   /* key 0x90 > 0x81 so node := end */
        g_readinessRet  = 0.9f;
        out = 0.0f;
        bool r = CGameScriptInterface_IsPlayerCreatureReadyToFireProjectileWeapon(&self, 0, &out);
        if (!r) { printf("FAIL sentinel true\n"); return 1; }
        if (g_readinessArg != (void*)0xEEEE) { printf("FAIL sentinel value\n"); return 1; }
    }

    /* ---- case 6: node == end -> use end value; readiness <= threshold -> false ---- */
    {
        IsPCReady_MapNode endnode; endnode.m_Key = 0; endnode.m_Value = (void*)0x2;
        IsPCReady_Creature* c = make_creature(0x02, 0x00, &endnode);
        g_creatureRet   = c;
        g_lowerBoundRet = &endnode;  /* node == end */
        g_readinessRet  = 0.5f;      /* == threshold, not > -> false */
        out = 123.0f;
        bool r = CGameScriptInterface_IsPlayerCreatureReadyToFireProjectileWeapon(&self, 0, &out);
        if (r) { printf("FAIL not-ready\n"); return 1; }
        if (out != 123.0f) { printf("FAIL out untouched\n"); return 1; }
        if (g_readinessArg != (void*)0x2) { printf("FAIL end value\n"); return 1; }
    }

    printf("FSE2_00899c10_TEST PASS\n");
    return 0;
}