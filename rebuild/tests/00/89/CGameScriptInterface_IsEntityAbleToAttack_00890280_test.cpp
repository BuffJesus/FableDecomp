#include <cstdio>
#include "engine/CScriptThing.h"

// Links against the landed source: drives the real root through a fake
// CScriptThing vtable (slot 0x2c) and a raw CThing image with its Dead bit at +0x91.
class CThing;

bool __stdcall CGameScriptInterface_IsEntityAbleToAttack(const CScriptThing* scriptThing);

static unsigned char g_thing[0xa8];
static CThing* g_ent = 0;
static char g_canAttack = 1;
static const CThing* g_canAttackArg = 0;

char __fastcall Entity_CanAttack(const CThing* self)
{
    g_canAttackArg = self;
    return g_canAttack;
}

static CThing* __fastcall GetEnt(const CScriptThing* self)
{
    (void)self;
    return g_ent;
}

int main()
{
    // Build a vtable with slot 0x2c/4 = 11 pointing at GetEnt
    void* vt[16];
    for (int i = 0; i < 16; ++i) vt[i] = 0;
    vt[0x2c / 4] = (void*)&GetEnt;
    CScriptThing obj;
    obj.__vftable = (void*)vt;
    obj.PImp_Data = 0;
    obj.PImp_Info = 0;
    for (unsigned i = 0; i < sizeof(g_thing); ++i) g_thing[i] = 0;
    CThing* ent = (CThing*)g_thing;

    // Case 1: null entity -> false
    g_ent = 0;
    if (CGameScriptInterface_IsEntityAbleToAttack(&obj) != false) { std::printf("FAIL null\n"); return 1; }

    // Case 2: entity with Dead bit (0x1 at +0x91) set -> false
    g_thing[0x91] = 0x1;
    g_ent = ent;
    g_canAttack = true;
    g_canAttackArg = 0;
    if (CGameScriptInterface_IsEntityAbleToAttack(&obj) != false) { std::printf("FAIL flag\n"); return 1; }
    if (g_canAttackArg != 0) { std::printf("FAIL flag called\n"); return 1; }

    // Case 3: entity flag clear, CanAttack true -> true
    g_thing[0x91] = 0x0;
    g_canAttack = true;
    if (CGameScriptInterface_IsEntityAbleToAttack(&obj) != true) { std::printf("FAIL true\n"); return 1; }
    if (g_canAttackArg != ent) { std::printf("FAIL arg\n"); return 1; }

    // Case 4: entity flag clear, CanAttack false -> false
    g_canAttack = false;
    if (CGameScriptInterface_IsEntityAbleToAttack(&obj) != false) { std::printf("FAIL false\n"); return 1; }

    // Case 5: flag 0x2 only (not 0x1) with CanAttack true -> true
    g_thing[0x91] = 0x2;
    g_canAttack = true;
    if (CGameScriptInterface_IsEntityAbleToAttack(&obj) != true) { std::printf("FAIL flag2\n"); return 1; }

    std::printf("CGameScriptInterface_00890280_TEST PASS\n");
    return 0;
}
