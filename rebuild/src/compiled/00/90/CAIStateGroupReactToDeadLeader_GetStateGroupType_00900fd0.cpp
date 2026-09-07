#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_ReactToDeadLeader.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_ReactToDeadLeader_Methods : CAIStateGroup_ReactToDeadLeader {
    int Get();
};
int CAIStateGroup_ReactToDeadLeader_Methods::Get() { return 1; }