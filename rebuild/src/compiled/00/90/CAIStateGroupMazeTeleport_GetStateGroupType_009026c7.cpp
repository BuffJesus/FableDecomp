#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_MazeTeleport.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MazeTeleport_Methods : CAIStateGroup_MazeTeleport {
    int Get();
};
int CAIStateGroup_MazeTeleport_Methods::Get() { return 1; }