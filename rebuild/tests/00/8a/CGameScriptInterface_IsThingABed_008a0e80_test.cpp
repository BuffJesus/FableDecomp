#include <cstdio>

struct Thing {
    unsigned char pad0[0x24];
    unsigned int field24;   // +0x24
    unsigned char pad28[0x91 - 0x28];
    unsigned char flags91;  // +0x91
};

struct CGameScriptInterface;
typedef Thing* (__fastcall *GetThingFn)(CGameScriptInterface*);
struct CGameScriptInterface {
    GetThingFn* vt;
};

bool __stdcall CGameScriptInterface_IsThingABed(CGameScriptInterface* self)
{
    Thing* t = self->vt[0x2c/4](self);
    if (t != 0 && !(t->flags91 & 0x1) && (t->field24 & 0x200000))
        return true;
    return false;
}

static Thing* g_thing;
static Thing* __fastcall get_thing(CGameScriptInterface*) { return g_thing; }
static GetThingFn g_vt[16];

int main() {
    g_vt[0x2c/4] = get_thing;
    CGameScriptInterface obj;
    obj.vt = g_vt;

    g_thing = 0;
    if (CGameScriptInterface_IsThingABed(&obj) != false) { std::printf("FAIL null\n"); return 1; }

    Thing t;
    t.field24 = 0x200000;
    t.flags91 = 1;
    g_thing = &t;
    if (CGameScriptInterface_IsThingABed(&obj) != false) { std::printf("FAIL flag91\n"); return 1; }

    t.flags91 = 0;
    t.field24 = 0;
    if (CGameScriptInterface_IsThingABed(&obj) != false) { std::printf("FAIL field24\n"); return 1; }

    t.field24 = 0x200000;
    t.flags91 = 0;
    if (CGameScriptInterface_IsThingABed(&obj) != true) { std::printf("FAIL true\n"); return 1; }

    t.field24 = 0xFFFFFFFF;
    t.flags91 = 0xFE;
    if (CGameScriptInterface_IsThingABed(&obj) != true) { std::printf("FAIL true2\n"); return 1; }

    std::printf("CGameScriptInterface_008a0e80_TEST PASS\n");
    return 0;
}