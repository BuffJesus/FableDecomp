#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_SeekEnemy.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_SeekEnemy_Methods : CAIStateGroup_SeekEnemy {
    int Get();
};
int CAIStateGroup_SeekEnemy_Methods::Get() { return 1; }