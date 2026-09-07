
#include "engine/CDecal.h"  // retyped onto the PDB layout; byte parity re-verified
void __fastcall CDecal_Dtor(CDecal* self);
void __fastcall CDecal_OpDelete(CDecal* self);

void __fastcall CDecal_Delete(CDecal* self)
{
    if (self)
    {
        CDecal_Dtor(self);
        CDecal_OpDelete(self);
    }
}