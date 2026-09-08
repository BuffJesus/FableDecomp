#include "engine/CAnimComponentSetCreatureMode.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentSetCreatureMode* (__fastcall* CloneCreatureModeFn)(CAnimComponentSetCreatureMode*);
CAnimComponentSetCreatureMode* __fastcall Clone(CAnimComponentSetCreatureMode* self)
{
    CloneCreatureModeFn* vtable = (CloneCreatureModeFn*)self->__vftable;
    CAnimComponentSetCreatureMode* copy = vtable[0x10 / 4](self);
    *(long*)copy->Mode = *(long*)self->Mode;
    return copy;
}
