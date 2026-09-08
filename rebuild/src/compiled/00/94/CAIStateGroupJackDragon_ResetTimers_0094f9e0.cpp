// CAIStateGroup_JackDragon_Methods::ResetTimers  @ 0x0094f9e0
// Faithful VC7.1 reconstruction.

#include "engine/CAIStateGroup_JackDragon.h"  // retyped onto the PDB layout; byte parity re-verified
int  GetCurrentTimeValue(void);        // 0x0049d870 : returns int in eax
void ResetHelper(void);                // 0x008fcfc0 : return discarded

struct CAIStateGroup_JackDragon_Methods : CAIStateGroup_JackDragon {
    // +0x58

    void ResetTimers();
};

void CAIStateGroup_JackDragon_Methods::ResetTimers()
{
    this->FramesInSnapRange = 0;
    ResetHelper();
    int t = GetCurrentTimeValue();
    this->FrameOfLastSlam = t;
    this->FrameWhenLastInRange = t;
}