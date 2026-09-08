#include "engine/CCombatAction_GenericStrikeResponseKnockdownRecoverBase.h"  // retyped onto the PDB layout; byte parity re-verified
typedef unsigned char (__fastcall* OverrideCheckFn)(CCombatAction_GenericStrikeResponseKnockdownRecoverBase*);
bool __fastcall IsOverrideableByActionOfSamePriority(
    CCombatAction_GenericStrikeResponseKnockdownRecoverBase* self)
{
    OverrideCheckFn* vtable = (OverrideCheckFn*)self->__vftable;
    return vtable[0xb4 / 4](self) ? false : true;
}
