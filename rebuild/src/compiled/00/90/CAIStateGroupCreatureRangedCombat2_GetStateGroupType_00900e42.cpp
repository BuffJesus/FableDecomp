#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_CreatureRangedCombat2.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_CreatureRangedCombat2_Methods : CAIStateGroup_CreatureRangedCombat2 {
    int Get();
};
int CAIStateGroup_CreatureRangedCombat2_Methods::Get() { return 1; }