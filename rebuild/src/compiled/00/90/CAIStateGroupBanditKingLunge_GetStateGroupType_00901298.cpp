#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_BanditKingLunge.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_BanditKingLunge_Methods : CAIStateGroup_BanditKingLunge {
    int Get();
};
int CAIStateGroup_BanditKingLunge_Methods::Get() { return 1; }