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