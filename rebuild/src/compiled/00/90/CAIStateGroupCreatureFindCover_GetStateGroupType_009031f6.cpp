#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_CreatureFindCover.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_CreatureFindCover_Methods : CAIStateGroup_CreatureFindCover {
    int Get();
};
int CAIStateGroup_CreatureFindCover_Methods::Get() { return 1; }