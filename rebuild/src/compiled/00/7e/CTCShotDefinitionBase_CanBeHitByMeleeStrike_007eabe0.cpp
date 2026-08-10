struct CTCShotDefinitionBase {
    char pad0[0xC];
    bool* pShot; // +0x0C
};

bool __fastcall CanBeHitByMeleeStrike(CTCShotDefinitionBase* self)
{
    return self->pShot[0x2C];
}