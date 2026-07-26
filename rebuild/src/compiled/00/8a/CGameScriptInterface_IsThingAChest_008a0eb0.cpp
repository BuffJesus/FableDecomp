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

bool __stdcall CGameScriptInterface_IsThingAChest(Thing* self)
{
    Thing* t = self->vt->getThing(self);
    if (t != 0
        && (t->flags91 & 0x1) == 0
        && (t->field24 & 0x10000000) != 0)
        return true;
    return false;
}