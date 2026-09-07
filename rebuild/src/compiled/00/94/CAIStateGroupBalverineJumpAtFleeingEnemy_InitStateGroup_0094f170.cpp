#include "engine/CAIStateGroup_BalverineJumpAtFleeingEnemy.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_BalverineJumpAtFleeingEnemy_Methods : CAIStateGroup_BalverineJumpAtFleeingEnemy {
    void M(int p0, int p1);
};
void CAIStateGroup_BalverineJumpAtFleeingEnemy_Methods::M(int p0, int p1){ this->WaitUntilWorldFrameForNextAttempt = 0u; }