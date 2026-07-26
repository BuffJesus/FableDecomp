struct CTCShotSub { char pad[0x49]; bool flag; };
struct CTCShotDefinitionBase { char pad[0x7a0]; CTCShotSub* sub; };
bool __fastcall CTCShotDefinitionBase_CanBeHitByMeleeStrike(const CTCShotDefinitionBase* self) {
    return self->sub->flag;
}