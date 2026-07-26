#include <cstdio>

struct CEntity;
struct CGameScriptInterface;

struct CGSIVtbl {
    void* slot0[11];
    CEntity* (__fastcall* getEntity)(CGameScriptInterface* self);
};
struct CGameScriptInterface {
    CGSIVtbl* vt;
};
struct CEntity {
    unsigned char _pad[0x91];
    unsigned char flags;
};

static bool g_meleeResult = true;
extern "C" bool __fastcall CheckMeleeWield(CEntity* e) { (void)e; return g_meleeResult; }

static CEntity* g_entity = 0;
struct Fake { CEntity* get() { return g_entity; } };

static CEntity* __fastcall getEntityImpl(CGameScriptInterface* self) { (void)self; return g_entity; }

bool __stdcall CGameScriptInterface_IsEntityWieldingMeleeWeapon(CGameScriptInterface* self)
{
    CEntity* e = self->vt->getEntity(self);
    if (e != 0 && (e->flags & 1) == 0)
        return CheckMeleeWield(e);
    return false;
}

int main()
{
    CGSIVtbl vt;
    vt.getEntity = (CEntity* (__fastcall*)(CGameScriptInterface*))getEntityImpl;
    CGameScriptInterface obj;
    obj.vt = &vt;

    // Case 1: null entity -> false
    g_entity = 0;
    if (CGameScriptInterface_IsEntityWieldingMeleeWeapon(&obj) != false) { std::printf("FAIL null\n"); return 1; }

    // Case 2: entity with flag bit0 set -> false
    static CEntity ent1;
    ent1.flags = 0x1;
    g_entity = &ent1;
    if (CGameScriptInterface_IsEntityWieldingMeleeWeapon(&obj) != false) { std::printf("FAIL flagset\n"); return 1; }

    // Case 3: entity clean, melee true -> true
    static CEntity ent2;
    ent2.flags = 0x0;
    g_entity = &ent2;
    g_meleeResult = true;
    if (CGameScriptInterface_IsEntityWieldingMeleeWeapon(&obj) != true) { std::printf("FAIL truecase\n"); return 1; }

    // Case 4: entity clean, melee false -> false
    g_meleeResult = false;
    if (CGameScriptInterface_IsEntityWieldingMeleeWeapon(&obj) != false) { std::printf("FAIL falsecase\n"); return 1; }

    // Case 5: flags with other bits but not bit0 -> proceeds
    static CEntity ent3;
    ent3.flags = 0x2;
    g_entity = &ent3;
    g_meleeResult = true;
    if (CGameScriptInterface_IsEntityWieldingMeleeWeapon(&obj) != true) { std::printf("FAIL otherbits\n"); return 1; }

    std::printf("CGameScriptInterface_00890220_TEST PASS\n");
    return 0;
}