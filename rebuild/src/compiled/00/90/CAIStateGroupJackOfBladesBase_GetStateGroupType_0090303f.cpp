#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_JackOfBladesBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_JackOfBladesBase_Methods : CAIStateGroup_JackOfBladesBase {
    int Get();
};
int CAIStateGroup_JackOfBladesBase_Methods::Get() { return 1; }