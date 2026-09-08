#pragma optimize("y",on)
#include "engine/CCreatureAction_GetOutOfBed.h"  // retyped onto the PDB layout; byte parity re-verified

struct CCreatureActionGetOutOfBedVTable {
    void (__fastcall *Slots[4])(void*);
};

extern void __fastcall CCreatureAction_Base_FinishAction(
    CCreatureAction_GetOutOfBed* self);

void __fastcall FinishAction(CCreatureAction_GetOutOfBed* self)
{
    if (!self->interrupted)
        ((CCreatureActionGetOutOfBedVTable*)self->__vftable)->Slots[3](self);
    CCreatureAction_Base_FinishAction(self);
}
