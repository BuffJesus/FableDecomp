#include "engine/CAIStateGroupBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIBrain { char pad[0x14]; unsigned long base; };

unsigned long* __fastcall CAIStateGroupBase_GetPWorldSeed(const CAIStateGroupBase* self)
{
    unsigned long v = self->PBrain->base;
    return (unsigned long*)(v + 0x1613c);
}