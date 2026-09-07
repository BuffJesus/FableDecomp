#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_VersusScorpionKing.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_VersusScorpionKing_Methods : CAIStateGroup_VersusScorpionKing {
    int Get();
};
int CAIStateGroup_VersusScorpionKing_Methods::Get() { return 1; }