#include "engine/CAnimComponentFollowingOnAnimFilter.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentFollowingOnAnimFilter* (__fastcall* CloneFollowingFilterFn)(CAnimComponentFollowingOnAnimFilter*);
CAnimComponentFollowingOnAnimFilter* __fastcall Clone(CAnimComponentFollowingOnAnimFilter* self)
{
    CloneFollowingFilterFn* vtable = (CloneFollowingFilterFn*)self->__vftable;
    CAnimComponentFollowingOnAnimFilter* copy = vtable[0x10 / 4](self);
    copy->NextFilter_TablePos = self->NextFilter_TablePos;
    return copy;
}
