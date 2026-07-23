struct Inner { char pad[0x18]; bool flag; };
struct CTCShotDefinitionBase { char pad[0x160]; Inner* def; };

bool __fastcall CTCShotDefinitionBase_CanBeHitByMeleeStrike(const CTCShotDefinitionBase* self)
{
    return self->def->flag;
}