#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_BriarRose.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_BriarRose_Methods : CAIStateGroup_BriarRose {
    int Get();
};
int CAIStateGroup_BriarRose_Methods::Get() { return 1; }