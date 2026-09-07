#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_CreatureUseCoverBase_Methods : CAIStateGroup_CreatureUseCoverBase {
    int Get();
};
int CAIStateGroup_CreatureUseCoverBase_Methods::Get() { return 1; }