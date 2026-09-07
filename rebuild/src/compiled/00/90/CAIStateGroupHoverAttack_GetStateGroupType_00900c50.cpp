#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_HoverAttack.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_HoverAttack_Methods : CAIStateGroup_HoverAttack {
    int Get();
};
int CAIStateGroup_HoverAttack_Methods::Get() { return 1; }