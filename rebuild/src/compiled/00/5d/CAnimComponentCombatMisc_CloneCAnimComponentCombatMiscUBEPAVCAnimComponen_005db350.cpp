#include "engine/CAnimComponentCombatMisc.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentCombatMisc* (__fastcall* CloneCombatMiscFn)(CAnimComponentCombatMisc*);
void* __fastcall Clone_CombatMisc(CAnimComponentCombatMisc* self)
{
    CloneCombatMiscFn* vtable = (CloneCombatMiscFn*)self->__vftable;
    CAnimComponentCombatMisc* copy = vtable[0x10 / 4](self);
    copy->MeleeFlourish = self->MeleeFlourish;
    copy->MeleeKnockdown = self->MeleeKnockdown;
    return copy;
}
