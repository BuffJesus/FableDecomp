#include "engine/CAnimComponentFollowingOnAnim.h"  // retyped onto the PDB layout; byte parity re-verified
typedef CAnimComponentFollowingOnAnim* (__fastcall* CloneFollowingAnimFn)(CAnimComponentFollowingOnAnim*);
CAnimComponentFollowingOnAnim* __fastcall Clone(CAnimComponentFollowingOnAnim* self)
{
    CloneFollowingAnimFn* vtable = (CloneFollowingAnimFn*)self->__vftable;
    CAnimComponentFollowingOnAnim* copy = vtable[0x10 / 4](self);
    copy->NextAnimName_TablePos = self->NextAnimName_TablePos;
    return copy;
}
