#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_Thunder.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_Thunder_Methods : CAIStateGroup_Thunder {
    int Get();
};
int CAIStateGroup_Thunder_Methods::Get() { return 1; }