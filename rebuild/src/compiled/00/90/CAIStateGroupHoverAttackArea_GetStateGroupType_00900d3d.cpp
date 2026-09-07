#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_HoverAttackArea.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_HoverAttackArea_Methods : CAIStateGroup_HoverAttackArea {
    int Get();
};
int CAIStateGroup_HoverAttackArea_Methods::Get() { return 1; }