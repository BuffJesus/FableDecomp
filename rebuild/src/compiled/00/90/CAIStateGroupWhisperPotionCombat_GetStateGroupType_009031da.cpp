#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_WhisperPotionCombat.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_WhisperPotionCombat_Methods : CAIStateGroup_WhisperPotionCombat {
    int Get();
};
int CAIStateGroup_WhisperPotionCombat_Methods::Get() { return 1; }