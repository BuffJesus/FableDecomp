#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_ScorpionKing.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_ScorpionKing_Methods : CAIStateGroup_ScorpionKing {
    int Get();
};
int CAIStateGroup_ScorpionKing_Methods::Get() { return 1; }