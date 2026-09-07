#include "engine/CThingGameObject.h"  // retyped onto the PDB layout; byte parity re-verified
struct CThingGameObject_Methods : CThingGameObject {
    void SetAddToComboMultiplierOnHit(bool v);
};

void CThingGameObject_Methods::SetAddToComboMultiplierOnHit(bool v)
{
    flags = (unsigned char)((flags & 0xfe) | (v & 1) | 2);
}