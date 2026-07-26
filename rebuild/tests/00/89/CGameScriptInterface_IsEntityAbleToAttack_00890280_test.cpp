#include <cstdio>

struct Entity {
    void* vt;
    unsigned char pad[0x91 - 4];
    unsigned char flags; // +0x91
};

struct CGameScriptInterface;
typedef Entity* (__fastcall* GetEntityFn)(CGameScriptInterface*);
struct CGameScriptInterface {
    void** vt;
};

static Entity* g_ent = 0;
static char g_canAttack = 1;

char __fastcall Entity_CanAttack(Entity* self)
{
    (void)self;
    return g_canAttack;
}

Entity* __fastcall GetEnt(CGameScriptInterface* self)
{
    (void)self;
    return g_ent;
}

bool __stdcall CGameScriptInterface_IsEntityAbleToAttack(CGameScriptInterface* self)
{
    Entity* e = ((GetEntityFn*)self->vt)[0x2c / 4](self);
    if (e != 0 && (e->flags & 0x1) == 0)
        return Entity_CanAttack(e);
    return false;
}

int main()
{
    // Build a vtable with slot 0x2c/4 = 11 pointing at GetEnt
    void* vt[16];
    for (int i = 0; i < 16; ++i) vt[i] = 0;
    vt[0x2c / 4] = (void*)&GetEnt;
    CGameScriptInterface obj;
    obj.vt = (void**)vt;

    // Case 1: null entity -> false
    g_ent = 0;
    if (CGameScriptInterface_IsEntityAbleToAttack(&obj) != false) { std::printf("FAIL null\n"); return 1; }

    // Case 2: entity with flag 0x1 set -> false
    Entity ent;
    for (unsigned i = 0; i < sizeof(ent.pad); ++i) ent.pad[i] = 0;
    ent.vt = 0;
    ent.flags = 0x1;
    g_ent = &ent;
    if (CGameScriptInterface_IsEntityAbleToAttack(&obj) != false) { std::printf("FAIL flag\n"); return 1; }

    // Case 3: entity flag clear, CanAttack true -> true
    ent.flags = 0x0;
    g_canAttack = true;
    if (CGameScriptInterface_IsEntityAbleToAttack(&obj) != true) { std::printf("FAIL true\n"); return 1; }

    // Case 4: entity flag clear, CanAttack false -> false
    g_canAttack = false;
    if (CGameScriptInterface_IsEntityAbleToAttack(&obj) != false) { std::printf("FAIL false\n"); return 1; }

    // Case 5: flag 0x2 only (not 0x1) with CanAttack true -> true
    ent.flags = 0x2;
    g_canAttack = true;
    if (CGameScriptInterface_IsEntityAbleToAttack(&obj) != true) { std::printf("FAIL flag2\n"); return 1; }

    std::printf("CGameScriptInterface_00890280_TEST PASS\n");
    return 0;
}