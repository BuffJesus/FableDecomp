#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_MazeShieldedChase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MazeShieldedChase_Methods : CAIStateGroup_MazeShieldedChase {
    int Get();
};
int CAIStateGroup_MazeShieldedChase_Methods::Get() { return 1; }