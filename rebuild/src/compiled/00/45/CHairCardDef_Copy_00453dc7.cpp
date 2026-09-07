
#include "engine/CHairCardDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CHairCardBase {
    void Copy(CHairCardDef* other);
};

void __fastcall CHairCardDef_Copy(CHairCardDef* self, int edx_dummy, CHairCardDef* other)
{
    (void)edx_dummy;
    reinterpret_cast<CHairCardBase*>(self)->Copy(other);
    self->HairObject = other->HairObject;
}