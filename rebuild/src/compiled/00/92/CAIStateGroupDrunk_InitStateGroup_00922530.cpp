#include "engine/CAIStateGroup_Drunk.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_Drunk_Methods : CAIStateGroup_Drunk {
    void M(int p0, int p1);
};
void CAIStateGroup_Drunk_Methods::M(int p0, int p1){ this->FrameToNextTalkToSelf = 0u; }