#include "engine/CResurrectionItemDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CSubField { int v; };
struct CBaseDrainLifeDef {
    void Copy(const CBaseDrainLifeDef* other);
};

void __fastcall CSpecialAbilitiesDrainLifeDataDef_Copy(CResurrectionItemDef* self, int _edx, const CResurrectionItemDef* other)
{
    ((CBaseDrainLifeDef*)self)->Copy((const CBaseDrainLifeDef*)other);
    self->OnUseParticleEffect = other->OnUseParticleEffect;
}