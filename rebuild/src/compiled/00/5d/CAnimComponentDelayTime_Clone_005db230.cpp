#include "engine/CAnimComponentDelayTime.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentDelayTime* (__fastcall* CloneDelayFn)(CAnimComponentDelayTime*);
CAnimComponentDelayTime* __fastcall Clone(CAnimComponentDelayTime* self)
{
    CloneDelayFn* vtable = (CloneDelayFn*)self->__vftable;
    CAnimComponentDelayTime* copy = vtable[0x10 / 4](self);
    copy->Delay = self->Delay;
    return copy;
}
