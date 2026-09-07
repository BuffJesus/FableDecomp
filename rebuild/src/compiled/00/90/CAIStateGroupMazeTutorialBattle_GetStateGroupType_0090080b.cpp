#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_MazeTutorialBattle.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MazeTutorialBattle_Methods : CAIStateGroup_MazeTutorialBattle {
    int Get();
};
int CAIStateGroup_MazeTutorialBattle_Methods::Get() { return 1; }