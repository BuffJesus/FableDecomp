#include "engine/CCreatureAction_ForcePushOutOf.h"  // retyped onto the PDB layout; byte parity re-verified

struct CCreatureActionForcePushVTable {
    void (__fastcall *Slots[4])(CCreatureAction_ForcePushOutOf*);
};

extern bool __fastcall CreatureForcePush_Check(CCreatureAction_ForcePushOutOf* self);
extern void __fastcall CreatureAction_FinishAction_Base(CCreatureAction_ForcePushOutOf* self);

void __fastcall CCreatureAction_ForcePushOutOf_FinishAction(
    CCreatureAction_ForcePushOutOf* self)
{
    if (!CreatureForcePush_Check(self)) {
        ((CCreatureActionForcePushVTable*)self->__vftable)->Slots[3](self);
    }
    CreatureAction_FinishAction_Base(self);
}
