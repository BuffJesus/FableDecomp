#include <cstdio>

static int g_ctor = 0;
static int g_dtor = 0;
static int g_vt_a = 0;
static int g_vt_entity = 0;
static int g_vt_enabled = 0;
static int g_ctor_x = 0;
static int g_ctor_y = 0;

struct Local {
    int a;
    Local(int x, int y){ a = x; g_ctor = 1; g_ctor_x = x; g_ctor_y = y; }
    ~Local(){ g_dtor = 1; }
};

struct CGameScriptInterface;
typedef void (__fastcall *PFN)(CGameScriptInterface*, Local*, int, Local*, int);
struct VT { PFN slots[0x300]; };
struct CGameScriptInterface { VT* vt; };

void __fastcall vimpl(CGameScriptInterface*, Local* edxLoc, int entity, Local* loc, int enabled)
{
    g_vt_a = edxLoc->a;
    g_vt_entity = entity;
    g_vt_enabled = enabled;
}

void __fastcall CGameScriptInterface_EntitySetDeedReactionsEnabled(CGameScriptInterface* self, int, int entity, int enabled)
{
    Local loc(0x1275988, -1);
    self->vt->slots[0x938/4](self, &loc, entity, &loc, enabled);
}

int main()
{
    static PFN vt[0x300];
    vt[0x938/4] = vimpl;
    VT v;
    for (int i = 0; i < 0x300; i++) v.slots[i] = vt[i];
    CGameScriptInterface obj; obj.vt = &v;

    CGameScriptInterface_EntitySetDeedReactionsEnabled(&obj, 0, 111, 222);

    if (g_ctor && g_dtor && g_ctor_x == 0x1275988 && g_ctor_y == -1 &&
        g_vt_a == 0x1275988 && g_vt_entity == 111 && g_vt_enabled == 222) {
        std::printf("CGameScriptInterface_0088f850_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL ctor=%d dtor=%d x=%d y=%d a=%d ent=%d en=%d\n",
                g_ctor, g_dtor, g_ctor_x, g_ctor_y, g_vt_a, g_vt_entity, g_vt_enabled);
    return 1;
}