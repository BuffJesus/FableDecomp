#pragma optimize("s",on)
#include "engine/CCombatAbilityAttackBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CPersistContext {
    void TByte(unsigned char* p);
    void TWord(unsigned short* p);
    void TSub(void* p);
};

// CombatAbility (+0x28, CCombatAbilityMeleeAttackData, 0x20 bytes) is opaque in the header;
// the serialised fields live inside it: sub-object at +0x00, bytes at +0x18/+0x19, word at +0x1c.
struct CCombatAbilityAttackBase_Methods : CCombatAbilityAttackBase {
    void Transfer(CPersistContext& ctx);
};

void CCombatAbilityAttackBase_Methods::Transfer(CPersistContext& ctx)
{
    ctx.TByte(&this->CombatAbility[0x18]);                     // +0x40
    ctx.TByte(&this->CombatAbility[0x19]);                     // +0x41
    ctx.TWord((unsigned short*)&this->CombatAbility[0x1c]);    // +0x44
    ctx.TSub(&this->CombatAbility[0x00]);                      // +0x28
}
