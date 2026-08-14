#include <cstdio>

struct CCharString { int id; };

struct Sub {
    int base;
    int resolve(CCharString* s) { return base + s->id; }
};

struct Obj { char pad[0x54]; Sub* p54; };
struct CTCEnemy { char pad[0x18]; int f18; };

static Obj g_obj;
static Sub g_sub;

Obj* __fastcall Enemy_GetObj(CTCEnemy* self) {
    (void)self;
    return &g_obj;
}

void __fastcall SetFaction(CTCEnemy* self, void* edx, CCharString* faction) {
    (void)edx;
    Obj* o = Enemy_GetObj(self);
    self->f18 = o->p54->resolve(faction);
}

int main() {
    g_sub.base = 1000;
    g_obj.p54 = &g_sub;
    CTCEnemy e; e.f18 = -1;
    CCharString cs; cs.id = 7;
    SetFaction(&e, 0, &cs);
    if (e.f18 == 1007) printf("FACTION_OK\n");
    else printf("FAIL %d\n", e.f18);
    return 0;
}