#include <cstdio>

struct CThing {
    unsigned char _pad0[0x24];
    unsigned char flag24;   // +0x24
    unsigned char _pad1[0x91 - 0x25];
    unsigned char flag91;   // +0x91
};

struct CGameScriptInterface;

struct CGameScriptInterfaceVtbl {
    unsigned char _pad[0x2c];
    CThing* (__fastcall *getThing)(CGameScriptInterface* self);
};

struct CGameScriptInterface {
    CGameScriptInterfaceVtbl* vt;
};

bool __stdcall CGameScriptInterface_IsThingADoor(CGameScriptInterface* self)
{
    CThing* t = self->vt->getThing(self);
    if (t && !(t->flag91 & 1) && (t->flag24 & 0x40))
        return true;
    return false;
}

static CThing g_thing;
static CThing* __fastcall get_thing(CGameScriptInterface* self){ (void)self; return &g_thing; }
static CThing* __fastcall get_null(CGameScriptInterface* self){ (void)self; return 0; }

int main(){
    CGameScriptInterfaceVtbl vt;
    CGameScriptInterface obj; obj.vt = &vt;

    // case: null thing -> false
    vt.getThing = get_null;
    if (CGameScriptInterface_IsThingADoor(&obj) != false){ std::printf("FAIL null\n"); return 1; }

    // case: flag91 bit0 set -> false
    vt.getThing = get_thing;
    g_thing.flag91 = 1; g_thing.flag24 = 0x40;
    if (CGameScriptInterface_IsThingADoor(&obj) != false){ std::printf("FAIL flag91\n"); return 1; }

    // case: flag24 bit6 clear -> false
    g_thing.flag91 = 0; g_thing.flag24 = 0x00;
    if (CGameScriptInterface_IsThingADoor(&obj) != false){ std::printf("FAIL flag24clear\n"); return 1; }

    // case: door -> true
    g_thing.flag91 = 0; g_thing.flag24 = 0x40;
    if (CGameScriptInterface_IsThingADoor(&obj) != true){ std::printf("FAIL door\n"); return 1; }

    // extra bits shouldn't matter
    g_thing.flag91 = 0xFE; g_thing.flag24 = 0xFF;
    if (CGameScriptInterface_IsThingADoor(&obj) != true){ std::printf("FAIL door2\n"); return 1; }

    std::printf("CGameScriptInterface_008a0ee0_TEST PASS\n");
    return 0;
}