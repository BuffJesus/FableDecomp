#pragma optimize("t",on)

struct CThingAICreature;

struct CReactionFrequencyTraits_PeriodicFrequency {
    char pad[0x1c];
    int result;
};

extern "C" int __fastcall Helper_LR(CReactionFrequencyTraits_PeriodicFrequency* self);

void __fastcall LogReaction(CReactionFrequencyTraits_PeriodicFrequency* self,
                            int edx_pad,
                            const CThingAICreature& c, bool flag)
{
    if (flag)
        self->result = Helper_LR(self);
}