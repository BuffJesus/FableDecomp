#include "engine/CCombatAbilityBlockCounterAttackDef.h"  // retyped onto the PDB layout; byte parity re-verified

// Base-class and +0x28 sub-object types are not expressed by the generated header
// (its bytes are flattened into _base_0x04), so they stay as local call-shape models.
struct CMember { void Copy(const CMember* src); };
struct CBaseDef { void Copy(const CBaseDef* src); };

void __fastcall CCombatAbilityBlockCounterAttackDef_Copy(CCombatAbilityBlockCounterAttackDef* self, void* edx, const CCombatAbilityBlockCounterAttackDef* src)
{
    ((CBaseDef*)self)->Copy((const CBaseDef*)src);
    CMember* m = (CMember*)(self->_base_0x04 + 0x24);
    m->Copy((const CMember*)(src->_base_0x04 + 0x24));
}
