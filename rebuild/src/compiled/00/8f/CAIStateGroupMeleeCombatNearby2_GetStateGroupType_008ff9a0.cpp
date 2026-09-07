#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_MeleeCombatNearby2.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MeleeCombatNearby2_Methods : CAIStateGroup_MeleeCombatNearby2 {
    int Get();
};
int CAIStateGroup_MeleeCombatNearby2_Methods::Get() { return 1; }