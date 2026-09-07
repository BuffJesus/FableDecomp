#include "engine/CCreatureAction_StabbedToDeath.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_StabbedToDeath_Methods : CCreatureAction_StabbedToDeath {
    int GetAnimationTransitionInTime();
};
int CCreatureAction_StabbedToDeath_Methods::GetAnimationTransitionInTime(){ return 0; }