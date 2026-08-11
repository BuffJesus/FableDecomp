struct CWorld;
struct CWorldVtbl {
    void (__fastcall *slots[0x10])(CWorld*);
    void (__fastcall *onDisable)(CWorld*); // slot 0x10 => [eax+0x40]
};
struct CWorld {
    CWorldVtbl* vtbl;   // +0x00
    char pad[0x24 - 4]; // +0x04 .. +0x23
    bool f24;           // +0x24
};

void __fastcall SetMiniMapEnabled(CWorld* self, int /*edx*/, bool enabled)
{
    if (self->f24 != enabled) {
        self->f24 = enabled;
        if (!enabled) {
            self->vtbl->onDisable(self);
        }
    }
}