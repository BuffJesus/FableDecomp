#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_VersusEnemyBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_VersusEnemyBase_Methods : CAIStateGroup_VersusEnemyBase {
    int Get();
};
int CAIStateGroup_VersusEnemyBase_Methods::Get() { return 1; }