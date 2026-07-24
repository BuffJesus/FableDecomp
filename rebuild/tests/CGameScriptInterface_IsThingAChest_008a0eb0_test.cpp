#include <cstdio>

struct Thing;
struct ThingVtbl {
    void* slot0;
    void* slot4;
    void* slot8;
    void* slotc;
    void* slot10;
    void* slot14;
    void* slot18;
    void* slot1c;
    void* slot20;
    void* slot24;
    void* slot28;
    Thing* (__fastcall* getThing)(void* self); // +0x2c
};
struct Thing {
    ThingVtbl* vt;      // +0x0
    unsigned char _p0[0x20];
    unsigned int field24; // +0x24
    unsigned char _p24[0x69]; // to 0x91
    unsigned char flags91; // +0x91
};

static Thing* g_ret;
static Thing* __fastcall thunk(void* self) { return g_ret; }

bool __stdcall CGameScriptInterface_IsThingAChest(Thing* self)
{
    Thing* t = self->vt->getThing(self);
    if (t != 0
        && (t->flags91 & 0x1) == 0
        && (t->field24 & 0x10000000) != 0)
        return true;
    return false;
}

int main()
{
    ThingVtbl vt;
    vt.getThing = (Thing* (__fastcall*)(void*))thunk;
    Thing self; self.vt = &vt;

    g_ret = 0;
    if (CGameScriptInterface_IsThingAChest(&self) != false) { std::printf("FAIL null\n"); return 1; }

    Thing t; t.vt = &vt;

    g_ret = &t;
    t.flags91 = 0x1; t.field24 = 0x10000000;
    if (CGameScriptInterface_IsThingAChest(&self) != false) { std::printf("FAIL flag91\n"); return 1; }

    t.flags91 = 0x0; t.field24 = 0x0;
    if (CGameScriptInterface_IsThingAChest(&self) != false) { std::printf("FAIL field24\n"); return 1; }

    t.flags91 = 0x0; t.field24 = 0x10000000;
    if (CGameScriptInterface_IsThingAChest(&self) != true) { std::printf("FAIL chest\n"); return 1; }

    t.flags91 = 0x2; t.field24 = 0x1000000F;
    if (CGameScriptInterface_IsThingAChest(&self) != true) { std::printf("FAIL mixed\n"); return 1; }

    std::printf("CGameScriptInterface_008a0eb0_TEST PASS\n");
    return 0;
}