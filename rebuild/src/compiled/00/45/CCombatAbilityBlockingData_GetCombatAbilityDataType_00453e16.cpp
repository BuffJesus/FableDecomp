#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CCombatAbilityBlockingData.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCombatAbilityBlockingData_Methods : CCombatAbilityBlockingData {
    int Get();
};
int CCombatAbilityBlockingData_Methods::Get() { return 1; }