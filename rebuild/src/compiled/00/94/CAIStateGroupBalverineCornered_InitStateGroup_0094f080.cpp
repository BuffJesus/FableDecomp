#include "engine/CAIStateGroup_BalverineCornered.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_BalverineCornered_Methods : CAIStateGroup_BalverineCornered {
    void M(int p0, int p1);
};
void CAIStateGroup_BalverineCornered_Methods::M(int p0, int p1){ this->TimeOfLastJump = 0u; }