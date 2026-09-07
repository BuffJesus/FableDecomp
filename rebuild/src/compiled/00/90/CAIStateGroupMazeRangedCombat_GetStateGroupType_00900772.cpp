#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_MazeRangedCombat.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MazeRangedCombat_Methods : CAIStateGroup_MazeRangedCombat {
    int Get();
};
int CAIStateGroup_MazeRangedCombat_Methods::Get() { return 1; }