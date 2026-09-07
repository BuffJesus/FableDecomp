#pragma optimize("s",on)
// Constant-return member: returns 60. __fastcall this=ecx.
#include "engine/CChestDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CChestDef_Methods : CChestDef {
    int Get();
};
int CChestDef_Methods::Get() { return 60; }