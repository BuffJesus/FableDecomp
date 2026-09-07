#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_CreatureTakeCover.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_CreatureTakeCover_Methods : CAIStateGroup_CreatureTakeCover {
    int Get();
};
int CAIStateGroup_CreatureTakeCover_Methods::Get() { return 1; }