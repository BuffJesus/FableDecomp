#include "engine/COccupiableDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CSubField { int v; };
struct CBaseDrainLifeDef {
    void Copy(const CBaseDrainLifeDef* other);
};

void __fastcall CSpecialAbilitiesDrainLifeDataDef_Copy(COccupiableDef* self, int _edx, const COccupiableDef* other)
{
    ((CBaseDrainLifeDef*)self)->Copy((const CBaseDrainLifeDef*)other);
    self->TypeFlags = other->TypeFlags;
}