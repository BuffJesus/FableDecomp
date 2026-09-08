#include "engine/CCombatAbilityFlourishCounterAttackDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CBaseDef {
    void Copy(const CBaseDef* other);
};

struct CMemberDef {
    void Copy(const CMemberDef* other);
};

struct CCombatAbilityFlourishCounterAttackDef_Methods : CCombatAbilityFlourishCounterAttackDef {
    void Copy(const CCombatAbilityFlourishCounterAttackDef* other);
};

void CCombatAbilityFlourishCounterAttackDef_Methods::Copy(
        const CCombatAbilityFlourishCounterAttackDef* other)
{
    ((CBaseDef*)this)->Copy((const CBaseDef*)other);
    // header types the +0x28 sub-object as `long m_member`; the retail code copies it via
    // the member class's Copy (ecx = this+0x28), so view the slot through that type here.
    ((CMemberDef*)&m_member)->Copy((const CMemberDef*)&other->m_member);
}
