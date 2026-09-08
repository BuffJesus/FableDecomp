#include "engine/CAnimComponentCombatHandedness.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentCombatHandedness* (__fastcall* CloneHandednessFn)(CAnimComponentCombatHandedness*);
CAnimComponentCombatHandedness* __fastcall Clone_CombatHandedness(CAnimComponentCombatHandedness* self)
{
    CloneHandednessFn* vtable = (CloneHandednessFn*)self->__vftable;
    CAnimComponentCombatHandedness* copy = vtable[0x10 / 4](self);
    *(long*)copy->StartHandedness = *(long*)self->StartHandedness;
    *(long*)copy->EndHandedness = *(long*)self->EndHandedness;
    return copy;
}
