struct CThing {
    unsigned char _pad0[0x24];
    unsigned char flag24;   // +0x24
    unsigned char _pad1[0x91 - 0x25];
    unsigned char flag91;   // +0x91
};

struct CGameScriptInterface;

struct CGameScriptInterfaceVtbl {
    unsigned char _pad[0x2c];
    CThing* (__fastcall *getThing)(CGameScriptInterface* self); // slot at +0x2c
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