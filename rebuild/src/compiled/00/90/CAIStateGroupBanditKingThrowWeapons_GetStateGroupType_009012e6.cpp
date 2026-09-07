#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_BanditKingThrowWeapons.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_BanditKingThrowWeapons_Methods : CAIStateGroup_BanditKingThrowWeapons {
    int Get();
};
int CAIStateGroup_BanditKingThrowWeapons_Methods::Get() { return 1; }