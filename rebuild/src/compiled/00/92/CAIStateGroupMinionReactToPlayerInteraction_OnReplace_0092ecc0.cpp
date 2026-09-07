// Clear the two byte flags at this+0x2c / this+0x2d. __fastcall this=ecx.
#include "engine/CAIStateGroup_MinionReactToPlayerInteraction.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
struct CAIStateGroup_MinionReactToPlayerInteraction_Methods : CAIStateGroup_MinionReactToPlayerInteraction {
    void Clear();
};
#pragma pack(pop)
void CAIStateGroup_MinionReactToPlayerInteraction_Methods::Clear() { this->WaitingForConfirmation = false; this->DidTraderInteraction = false; }