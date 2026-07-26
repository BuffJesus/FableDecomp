#include <cstdio>

static int g_applied = 0;
static int g_last = -999;

struct SGObj {
    void* vt;
    char pad4[0xc];
    char kind;
    void Apply(int enabled);
};
void SGObj::Apply(int enabled) { g_applied++; g_last = enabled; }

struct Entity {
    void* vt;
};

typedef SGObj* (__fastcall *GetSGFn)(Entity*);

void __stdcall CGameScriptInterface_EntitySetAllStategroupsEnabled(Entity* entity, int enabled)
{
    SGObj* o = (*(GetSGFn*)((char*)(*(void**)entity) + 0x2c))(entity);
    if (o == 0) return;
    if (o->kind != 1) return;
    o->Apply(enabled);
}

static SGObj g_obj;
static SGObj* g_ret;

static SGObj* __fastcall getsg_thunk(Entity* e) { (void)e; return g_ret; }

int main() {
    static void* vt[16];
    vt[11] = (void*)&getsg_thunk;
    Entity ent; ent.vt = (void*)vt;

    g_ret = 0; g_applied = 0;
    CGameScriptInterface_EntitySetAllStategroupsEnabled(&ent, 1);
    if (g_applied != 0) { std::printf("FAIL null case\n"); return 1; }

    g_obj.kind = 2; g_ret = &g_obj; g_applied = 0;
    CGameScriptInterface_EntitySetAllStategroupsEnabled(&ent, 1);
    if (g_applied != 0) { std::printf("FAIL kind case\n"); return 1; }

    g_obj.kind = 1; g_ret = &g_obj; g_applied = 0; g_last = -1;
    CGameScriptInterface_EntitySetAllStategroupsEnabled(&ent, 7);
    if (g_applied != 1 || g_last != 7) { std::printf("FAIL apply case\n"); return 1; }

    std::printf("CGameScriptInterface_0088f6e0_TEST PASS\n");
    return 0;
}