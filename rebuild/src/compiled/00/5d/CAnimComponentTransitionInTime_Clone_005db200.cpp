#include "engine/CAnimComponentTransitionInTime.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentTransitionInTime* (__fastcall* CloneTransitionFn)(CAnimComponentTransitionInTime*);
CAnimComponentTransitionInTime* __fastcall Clone(CAnimComponentTransitionInTime* self)
{
    CloneTransitionFn* vtable = (CloneTransitionFn*)self->__vftable;
    CAnimComponentTransitionInTime* copy = vtable[0x10 / 4](self);
    copy->TransitionInTime = self->TransitionInTime;
    return copy;
}
