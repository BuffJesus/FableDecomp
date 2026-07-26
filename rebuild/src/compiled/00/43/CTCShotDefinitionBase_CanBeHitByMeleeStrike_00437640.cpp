struct SShotSub { char pad[0x18]; bool flag; };
struct CTCShotDefinitionBase { char pad[0x218]; SShotSub* sub; };

bool __fastcall CTCShotDefinitionBase_CanBeHitByMeleeStrike(const CTCShotDefinitionBase* self)
{
    return self->sub->flag;
}