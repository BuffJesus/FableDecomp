struct Inner { char pad[0x14]; unsigned long base; };
struct CAIStateGroupBase { char pad0[4]; Inner* p; };

unsigned long* __fastcall CAIStateGroupBase_GetPWorldSeed(const CAIStateGroupBase* self)
{
    unsigned long v = self->p->base;
    return (unsigned long*)(v + 0x1613c);
}