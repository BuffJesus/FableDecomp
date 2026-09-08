#include "engine/CAnimComponentFlags.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentFlags* (__fastcall* CloneFlagsFn)(CAnimComponentFlags*);
CAnimComponentFlags* __fastcall Clone(CAnimComponentFlags* self)
{
    CloneFlagsFn* vtable = (CloneFlagsFn*)self->__vftable;
    CAnimComponentFlags* copy = vtable[0x10 / 4](self);
    copy->Flags = self->Flags;
    return copy;
}
