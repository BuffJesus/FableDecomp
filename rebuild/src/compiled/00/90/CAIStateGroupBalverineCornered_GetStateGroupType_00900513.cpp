#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_BalverineCornered.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_BalverineCornered_Methods : CAIStateGroup_BalverineCornered {
    int Get();
};
int CAIStateGroup_BalverineCornered_Methods::Get() { return 1; }