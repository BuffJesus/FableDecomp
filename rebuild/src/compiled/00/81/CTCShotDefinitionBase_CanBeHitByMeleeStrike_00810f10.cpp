struct Inner {
    char pad[0x1b];
    bool flag;
};
struct CTCShotDefinitionBase {
    char pad0[8];
    Inner* f8;
};
bool __fastcall CanBeHitByMeleeStrike(CTCShotDefinitionBase* self) {
    return self->f8->flag;
}