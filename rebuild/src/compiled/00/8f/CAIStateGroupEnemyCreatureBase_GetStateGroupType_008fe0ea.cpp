#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_EnemyCreatureBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_EnemyCreatureBase_Methods : CAIStateGroup_EnemyCreatureBase {
    int Get();
};
int CAIStateGroup_EnemyCreatureBase_Methods::Get() { return 1; }