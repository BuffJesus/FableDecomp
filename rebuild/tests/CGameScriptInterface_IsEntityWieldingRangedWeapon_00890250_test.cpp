#include <cstdio>
struct Entity;
struct GSI_vt {
    void* slot[0xb];
    Entity* (__fastcall *getEntity)(void*); // +0x2c
};
struct Entity {
    unsigned char _pad[0x91];
    unsigned char flags; // +0x91
};

static Entity g_ent;
static bool g_wieldRet = true;

bool __fastcall EntityWieldsRanged(Entity* e) { (void)e; return g_wieldRet; }

struct GSI {
    GSI_vt* vt;
    Entity* ret;
};

static Entity* __fastcall GetEnt(void* self) { return ((GSI*)self)->ret; }

bool __stdcall CGameScriptInterface_IsEntityWieldingRangedWeapon(void* self)
{
    GSI_vt** vt = (GSI_vt**)self;
    Entity* e = (*vt)->getEntity(self);
    if (e != 0 && (((*(unsigned char*)((char*)e + 0x91)) & 1) == 0))
        return EntityWieldsRanged(e);
    return false;
}

int main() {
    GSI_vt vt; vt.getEntity = (Entity*(__fastcall*)(void*))GetEnt;
    GSI obj; obj.vt = &vt;

    // null entity -> false
    obj.ret = 0;
    if (CGameScriptInterface_IsEntityWieldingRangedWeapon(&obj) != false) { std::printf("FAIL null\n"); return 1; }

    // entity with flag bit set -> false
    g_ent.flags = 0x1;
    obj.ret = &g_ent;
    g_wieldRet = true;
    if (CGameScriptInterface_IsEntityWieldingRangedWeapon(&obj) != false) { std::printf("FAIL flagset\n"); return 1; }

    // entity, flag clear, helper true -> true
    g_ent.flags = 0x0;
    g_wieldRet = true;
    if (CGameScriptInterface_IsEntityWieldingRangedWeapon(&obj) != true) { std::printf("FAIL true\n"); return 1; }

    // entity, flag clear, helper false -> false
    g_wieldRet = false;
    if (CGameScriptInterface_IsEntityWieldingRangedWeapon(&obj) != false) { std::printf("FAIL false\n"); return 1; }

    std::printf("CGameScriptInterface_00890250_TEST PASS\n");
    return 0;
}