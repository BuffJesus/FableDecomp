#include "engine/CAnimComponentCombatRecoil.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentCombatRecoil* (__fastcall* CloneRecoilFn)(CAnimComponentCombatRecoil*);
CAnimComponentCombatRecoil* __fastcall Clone(CAnimComponentCombatRecoil* self)
{
    CloneRecoilFn* vtable = (CloneRecoilFn*)self->__vftable;
    CAnimComponentCombatRecoil* copy = vtable[0x10 / 4](self);
    copy->RecoilAnimIndex = self->RecoilAnimIndex;
    return copy;
}
