#include <cstdio>

struct Thing {
    unsigned char _pad0[0x28];
    unsigned int flags28;   // +0x28
    unsigned char _pad2[0x65];
    unsigned char flags91;  // +0x91
};

struct IGSIVtbl {
    void* slots[11];
    Thing* (__fastcall* getThing)(void* self);
};

struct CGameScriptInterface {
    IGSIVtbl* vt;
};

bool __stdcall CGameScriptInterface_IsThingSmashable(CGameScriptInterface* self)
{
    Thing* t = self->vt->getThing(self);
    if (t && !(t->flags91 & 0x1) && (t->flags28 & 0x4000))
        return true;
    return false;
}

static Thing* g_thing = 0;
static Thing* __fastcall getThingImpl(void* self) { (void)self; return g_thing; }

int main()
{
    IGSIVtbl vt;
    for (int i = 0; i < 11; ++i) vt.slots[i] = 0;
    vt.getThing = &getThingImpl;
    CGameScriptInterface obj;
    obj.vt = &vt;

    // null thing -> false
    g_thing = 0;
    if (CGameScriptInterface_IsThingSmashable(&obj)) { std::printf("FAIL null\n"); return 1; }

    Thing t;
    for (unsigned i = 0; i < sizeof(t); ++i) ((unsigned char*)&t)[i] = 0;
    g_thing = &t;

    // flags91 bit0 set -> false
    t.flags91 = 0x1; t.flags28 = 0x4000;
    if (CGameScriptInterface_IsThingSmashable(&obj)) { std::printf("FAIL flag91\n"); return 1; }

    // flags28 bit 0x4000 clear -> false
    t.flags91 = 0x0; t.flags28 = 0x0;
    if (CGameScriptInterface_IsThingSmashable(&obj)) { std::printf("FAIL flag28clear\n"); return 1; }

    // smashable
    t.flags91 = 0x0; t.flags28 = 0x4000;
    if (!CGameScriptInterface_IsThingSmashable(&obj)) { std::printf("FAIL smashable\n"); return 1; }

    // other bits of flags91 don't matter
    t.flags91 = 0xFE; t.flags28 = 0x4000;
    if (!CGameScriptInterface_IsThingSmashable(&obj)) { std::printf("FAIL flag91other\n"); return 1; }

    std::printf("CGameScriptInterface_008a0f10_TEST PASS\n");
    return 0;
}