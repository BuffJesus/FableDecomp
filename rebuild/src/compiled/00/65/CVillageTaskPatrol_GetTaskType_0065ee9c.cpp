#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CVillageTaskPatrol.h"  // retyped onto the PDB layout; byte parity re-verified
struct CVillageTaskPatrol_Methods : CVillageTaskPatrol {
    int Get();
};
int CVillageTaskPatrol_Methods::Get() { return 1; }