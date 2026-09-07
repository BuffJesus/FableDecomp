#include "engine/CAIStateGroup_MinionGuardReactToBribe.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MinionGuardReactToBribe_Methods : CAIStateGroup_MinionGuardReactToBribe {
    void M(int a0, int a1);
};
void CAIStateGroup_MinionGuardReactToBribe_Methods::M(int a0, int a1){ this->FrameLastReactedToReceiveing = 0x80000000; }