#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_WhisperRetreat.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_WhisperRetreat_Methods : CAIStateGroup_WhisperRetreat {
    int Get();
};
int CAIStateGroup_WhisperRetreat_Methods::Get() { return 1; }