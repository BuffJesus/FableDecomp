#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_SummonerSummon.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_SummonerSummon_Methods : CAIStateGroup_SummonerSummon {
    int Get();
};
int CAIStateGroup_SummonerSummon_Methods::Get() { return 1; }