#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_SummonerCharge.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_SummonerCharge_Methods : CAIStateGroup_SummonerCharge {
    int Get();
};
int CAIStateGroup_SummonerCharge_Methods::Get() { return 1; }