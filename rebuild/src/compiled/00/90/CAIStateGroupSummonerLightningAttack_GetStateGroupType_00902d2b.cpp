#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_SummonerLightningAttack.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_SummonerLightningAttack_Methods : CAIStateGroup_SummonerLightningAttack {
    int Get();
};
int CAIStateGroup_SummonerLightningAttack_Methods::Get() { return 1; }