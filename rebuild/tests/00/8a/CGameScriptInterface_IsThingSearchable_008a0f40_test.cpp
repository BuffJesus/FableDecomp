#include <cstdio>

struct CThing {
    unsigned char _pad0[0x24];
    unsigned int flags24; // +0x24
    unsigned char _pad2[0x91-0x28];
    unsigned char flags91; // +0x91
};

struct CGameScriptInterface {
    void** vt; // +0x0
};

typedef CThing* (__fastcall *GetThingFn)(const CGameScriptInterface*);

bool __stdcall CGameScriptInterface_IsThingSearchable(const CGameScriptInterface* self)
{
    CThing* t = ((GetThingFn)self->vt[0x2c/4])(self);
    if (t != 0 && (t->flags91 & 0x1) == 0 && (t->flags24 & 0x10000000) != 0)
        return true;
    return false;
}

static CThing* g_thing;
static CThing* __fastcall get_thing(const CGameScriptInterface*) { return g_thing; }

int main() {
    void* vtbl[16];
    vtbl[0x2c/4] = (void*)&get_thing;
    CGameScriptInterface obj;
    obj.vt = (void**)vtbl;

    // null thing -> false
    g_thing = 0;
    if (CGameScriptInterface_IsThingSearchable(&obj)) { std::printf("FAIL null\n"); return 1; }

    CThing t;
    for (int i=0;i<(int)sizeof(t);++i) ((unsigned char*)&t)[i]=0;
    g_thing = &t;

    // flags91 bit0 set -> false
    t.flags91 = 0x1; t.flags24 = 0x10000000;
    if (CGameScriptInterface_IsThingSearchable(&obj)) { std::printf("FAIL bit91\n"); return 1; }

    // flags24 bit missing -> false
    t.flags91 = 0; t.flags24 = 0;
    if (CGameScriptInterface_IsThingSearchable(&obj)) { std::printf("FAIL bit24\n"); return 1; }

    // both good -> true
    t.flags91 = 0; t.flags24 = 0x10000000;
    if (!CGameScriptInterface_IsThingSearchable(&obj)) { std::printf("FAIL good\n"); return 1; }

    std::printf("CGameScriptInterface_008a0f40_TEST PASS\n");
    return 0;
}