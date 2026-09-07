#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_MazeDefensiveMeleeCombat.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MazeDefensiveMeleeCombat_Methods : CAIStateGroup_MazeDefensiveMeleeCombat {
    int Get();
};
int CAIStateGroup_MazeDefensiveMeleeCombat_Methods::Get() { return 1; }