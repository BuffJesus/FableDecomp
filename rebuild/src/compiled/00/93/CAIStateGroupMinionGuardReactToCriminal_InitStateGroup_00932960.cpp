#include "engine/CAIStateGroup_MinionGuardReactToCriminal.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorld;
struct CThingAICreature;

struct CAIStateGroup_MinionGuardReactToCriminal_Methods : CAIStateGroup_MinionGuardReactToCriminal {
    // +0x30
    void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroup_MinionGuardReactToCriminal_Methods::InitStateGroup(
        const CWorld& world, CThingAICreature& creature)
{
    this->WarningsGiven = 0u;
    this->FrameWarningStarted = 0xe0000000u;
    this->DeedLastWarnedAbout = 0xffffffffu;
}