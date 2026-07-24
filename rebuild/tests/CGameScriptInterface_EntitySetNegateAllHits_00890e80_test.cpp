#include <cstdio>

struct Entity {
    unsigned char _p0[0x6c];
    unsigned char f6c;
    unsigned char _p6d[0x24];
    unsigned char f91;
    unsigned char _p92[0x2a];
    unsigned char fbc;
};

static Entity* g_ent;
struct GSI;
struct GSIvt {
    unsigned char _s0[0x2c];
    Entity* (__fastcall *getEntity)(void* self);
};
struct GSI { GSIvt* vt; };

Entity* __fastcall vfunc(void* self) { return g_ent; }

void __stdcall CGameScriptInterface_EntitySetNegateAllHits(GSI* self, unsigned char value)
{
    Entity* e = self->vt->getEntity(self);
    if (!e) return;
    if (e->f91 & 1) return;
    if (!(e->f6c & 2)) return;
    unsigned char cl = (unsigned char)(value << 4);
    cl ^= e->fbc;
    cl &= 0x10;
    e->fbc ^= cl;
}

int main() {
    GSIvt vt;
    for (int i=0;i<(int)sizeof(vt);i++) ((unsigned char*)&vt)[i]=0;
    vt.getEntity = &vfunc;
    GSI self; self.vt = &vt;

    Entity ent;
    for (int i=0;i<(int)sizeof(Entity);i++) ((unsigned char*)&ent)[i]=0;
    ent.f6c = 2;
    g_ent = &ent;

    CGameScriptInterface_EntitySetNegateAllHits(&self, 1);
    if ((ent.fbc & 0x10) != 0x10) { std::printf("FAIL set\n"); return 1; }
    CGameScriptInterface_EntitySetNegateAllHits(&self, 0);
    if ((ent.fbc & 0x10) != 0) { std::printf("FAIL clear\n"); return 1; }

    ent.f91 = 1; ent.fbc = 0;
    CGameScriptInterface_EntitySetNegateAllHits(&self, 1);
    if (ent.fbc != 0) { std::printf("FAIL blocked91\n"); return 1; }

    ent.f91 = 0; ent.f6c = 0; ent.fbc = 0;
    CGameScriptInterface_EntitySetNegateAllHits(&self, 1);
    if (ent.fbc != 0) { std::printf("FAIL blocked6c\n"); return 1; }

    g_ent = 0;
    CGameScriptInterface_EntitySetNegateAllHits(&self, 1);

    std::printf("CGameScriptInterface_00890e80_TEST PASS\n");
    return 0;
}