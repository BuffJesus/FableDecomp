#include "engine/CReactionFrequencyTraits_LinearTime.h"  // retyped onto the PDB layout; byte parity re-verified
struct CReactionFrequencyTraits_LinearTime; extern int __fastcall lr_helper(CReactionFrequencyTraits_LinearTime*);
struct CThingAICreature; struct CReactionFrequencyTraits_LinearTime_Methods : CReactionFrequencyTraits_LinearTime {
    void LogReaction(const CThingAICreature&, bool);
};
void CReactionFrequencyTraits_LinearTime_Methods::LogReaction(const CThingAICreature&, bool){ this->FrameHappened = lr_helper(this); }