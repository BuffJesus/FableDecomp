#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_HoverHornetQueen.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_HoverHornetQueen_Methods : CAIStateGroup_HoverHornetQueen {
    int Get();
};
int CAIStateGroup_HoverHornetQueen_Methods::Get() { return 1; }