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