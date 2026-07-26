#include <cstdio>

struct Entity {
    unsigned char pad0[0x6c];
    unsigned char f6c;
    unsigned char pad6d[0x91 - 0x6d];
    unsigned char f91;
    unsigned char pad92[0xbc - 0x92];
    unsigned char fbc;
};

struct GSIVtbl {
    void* s0; void* s1; void* s2; void* s3;
    void* s4; void* s5; void* s6; void* s7;
    void* s8; void* s9; void* sa;
    Entity* (__fastcall *getEntity)(void*);
};

struct CGameScriptInterface { GSIVtbl* vt; };

static Entity g_ent;
static Entity* __fastcall getent(void* self) { (void)self; return &g_ent; }

void __stdcall CGameScriptInterface_EntitySetAbleToBeEngagedInCombat(CGameScriptInterface* self, char enable)
{
    Entity* e = self->vt->getEntity(self);
    if (!e) return;
    if (e->f91 & 1) return;
    if (!(e->f6c & 2)) return;
    unsigned char c = (unsigned char)(enable << 5);
    e->fbc ^= (unsigned char)((c ^ e->fbc) & 0x20);
}

int main() {
    GSIVtbl vt;
    vt.s0=vt.s1=vt.s2=vt.s3=0;
    vt.s4=vt.s5=vt.s6=vt.s7=0;
    vt.s8=vt.s9=vt.sa=0;
    vt.getEntity = &getent;
    CGameScriptInterface obj;
    obj.vt = &vt;

    g_ent.f6c = 0x02; g_ent.f91 = 0; g_ent.fbc = 0x00;
    CGameScriptInterface_EntitySetAbleToBeEngagedInCombat(&obj, 1);
    if (g_ent.fbc != 0x20) { std::printf("FAIL set got 0x%x\n", g_ent.fbc); return 1; }

    CGameScriptInterface_EntitySetAbleToBeEngagedInCombat(&obj, 0);
    if (g_ent.fbc != 0x00) { std::printf("FAIL clear got 0x%x\n", g_ent.fbc); return 1; }

    g_ent.f91 = 1; g_ent.fbc = 0xAA;
    CGameScriptInterface_EntitySetAbleToBeEngagedInCombat(&obj, 1);
    if (g_ent.fbc != 0xAA) { std::printf("FAIL f91 guard\n"); return 1; }

    g_ent.f91 = 0; g_ent.f6c = 0; g_ent.fbc = 0x55;
    CGameScriptInterface_EntitySetAbleToBeEngagedInCombat(&obj, 1);
    if (g_ent.fbc != 0x55) { std::printf("FAIL f6c guard\n"); return 1; }

    std::printf("CGameScriptInterface_00890f00_TEST PASS\n");
    return 0;
}