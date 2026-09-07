
#include "engine/CThingGameObject.h"  // retyped onto the PDB layout; byte parity re-verified
void __fastcall SetAsGivingHeroStatChangesOnBeingHit(CThingGameObject *self, int edx_dummy, bool value)
{
    self->flags = (unsigned char)((((value & 1) | 2) << 2) | (self->flags & 0xfb));
}