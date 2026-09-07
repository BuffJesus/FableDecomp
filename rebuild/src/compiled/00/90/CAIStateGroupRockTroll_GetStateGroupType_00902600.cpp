#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_RockTroll.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_RockTroll_Methods : CAIStateGroup_RockTroll {
    int Get();
};
int CAIStateGroup_RockTroll_Methods::Get() { return 1; }