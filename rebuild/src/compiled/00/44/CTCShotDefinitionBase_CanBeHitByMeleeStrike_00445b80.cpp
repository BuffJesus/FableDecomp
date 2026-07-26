struct CInner { char pad[0x48]; bool flag; };
struct CTCShotDefinitionBase { char pad[0x7a0]; CInner* inner; };

bool __fastcall CTCShotDefinitionBase_CanBeHitByMeleeStrike(const CTCShotDefinitionBase* self)
{
    return self->inner->flag;
}