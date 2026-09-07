#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_CreatureAdvanceInCover.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_CreatureAdvanceInCover_Methods : CAIStateGroup_CreatureAdvanceInCover {
    int Get();
};
int CAIStateGroup_CreatureAdvanceInCover_Methods::Get() { return 1; }