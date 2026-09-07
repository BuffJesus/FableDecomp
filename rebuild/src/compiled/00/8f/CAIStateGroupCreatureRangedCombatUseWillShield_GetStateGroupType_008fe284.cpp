#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_CreatureRangedCombatUseWillShield.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_CreatureRangedCombatUseWillShield_Methods : CAIStateGroup_CreatureRangedCombatUseWillShield {
    int Get();
};
int CAIStateGroup_CreatureRangedCombatUseWillShield_Methods::Get() { return 1; }