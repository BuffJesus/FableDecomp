#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_BanditKingCharge.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_BanditKingCharge_Methods : CAIStateGroup_BanditKingCharge {
    int Get();
};
int CAIStateGroup_BanditKingCharge_Methods::Get() { return 1; }