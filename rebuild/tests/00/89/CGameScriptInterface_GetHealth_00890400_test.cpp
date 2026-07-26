#include <cstdio>

static float g_defaultHealth = 42.5f;

struct HeroObj {
    unsigned char _pad_0x6c[0x6c];
    unsigned char flag6c; // +0x6c
    unsigned char _pad_0x6d[0x24]; // to 0x91
    unsigned char flag91; // +0x91
    unsigned char _pad_0x92[0x22]; // to 0xb4
    float health; // +0xb4
};

struct GSI;
typedef HeroObj* (__fastcall *GetObjFn)(GSI*);
struct GSIVT {
    void* slots0[11];
    GetObjFn getObj;
};
struct GSI {
    GSIVT* vt;
};

static HeroObj g_obj;
static HeroObj* __fastcall retObj(GSI*) { return &g_obj; }
static HeroObj* __fastcall retNull(GSI*) { return 0; }

static GSIVT g_vt_obj;
static GSIVT g_vt_null;

float __stdcall CGameScriptInterface_GetHealth(GSI* self)
{
    HeroObj* o = self->vt->getObj(self);
    if (o != 0 &&
        (o->flag91 & 0x1) == 0 &&
        (o->flag6c & 0x2) != 0) {
        return o->health;
    }
    return g_defaultHealth;
}

int main() {
    g_vt_obj.getObj = retObj;
    g_vt_null.getObj = retNull;

    GSI a; a.vt = &g_vt_obj;
    GSI b; b.vt = &g_vt_null;

    g_obj.flag91 = 0x00;
    g_obj.flag6c = 0x02;
    g_obj.health = 123.0f;
    if (CGameScriptInterface_GetHealth(&a) != 123.0f) { std::printf("FAIL health\n"); return 1; }

    if (CGameScriptInterface_GetHealth(&b) != g_defaultHealth) { std::printf("FAIL null\n"); return 1; }

    g_obj.flag91 = 0x01;
    if (CGameScriptInterface_GetHealth(&a) != g_defaultHealth) { std::printf("FAIL flag91\n"); return 1; }

    g_obj.flag91 = 0x00;
    g_obj.flag6c = 0x00;
    if (CGameScriptInterface_GetHealth(&a) != g_defaultHealth) { std::printf("FAIL flag6c\n"); return 1; }

    std::printf("CGameScriptInterface_00890400_TEST PASS\n");
    return 0;
}