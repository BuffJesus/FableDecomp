#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_Guildmaster.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_Guildmaster_Methods : CAIStateGroup_Guildmaster {
    int Get();
};
int CAIStateGroup_Guildmaster_Methods::Get() { return 1; }