struct Thing {
    unsigned char _pad0[0x28];
    unsigned int flags28;   // +0x28
    unsigned char _pad2[0x65];
    unsigned char flags91;  // +0x91
};

struct IGSIVtbl {
    void* slots[11];
    Thing* (__fastcall* getThing)(void* self); // slot 0x2c/4 = 11
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