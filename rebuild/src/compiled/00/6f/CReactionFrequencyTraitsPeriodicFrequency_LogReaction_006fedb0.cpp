#pragma optimize("t",on)

#include "engine/CReactionFrequencyTraits_PeriodicFrequency.h"  // retyped onto the PDB layout; byte parity re-verified
struct CThingAICreature;


extern "C" int __fastcall Helper_LR(CReactionFrequencyTraits_PeriodicFrequency* self);

void __fastcall LogReaction(CReactionFrequencyTraits_PeriodicFrequency* self,
                            int edx_pad,
                            const CThingAICreature& c, bool flag)
{
    if (flag)
        self->LastHappenedFrame = Helper_LR(self);
}