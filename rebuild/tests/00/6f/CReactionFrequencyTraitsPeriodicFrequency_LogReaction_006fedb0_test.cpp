#include <cstdio>

struct CThingAICreature;

struct CReactionFrequencyTraits_PeriodicFrequency {
    char pad[0x1c];
    int result;
};

extern "C" int __fastcall Helper_LR(CReactionFrequencyTraits_PeriodicFrequency* self);

void __fastcall LogReaction(CReactionFrequencyTraits_PeriodicFrequency* self,
                            int edx_pad,
                            const CThingAICreature& c, bool flag);

static int g_called = 0;
extern "C" int __fastcall Helper_LR(CReactionFrequencyTraits_PeriodicFrequency* self)
{
    g_called++;
    return 0x1234;
}

int main()
{
    CReactionFrequencyTraits_PeriodicFrequency obj;
    obj.result = -1;
    const CThingAICreature* cp = 0;

    LogReaction(&obj, 0, *cp, false);
    if (g_called != 0) { printf("FAIL called on false\n"); return 1; }
    if (obj.result != -1) { printf("FAIL result changed on false\n"); return 1; }

    LogReaction(&obj, 0, *cp, true);
    if (g_called != 1) { printf("FAIL not called on true\n"); return 1; }
    if (obj.result != 0x1234) { printf("FAIL result not stored\n"); return 1; }

    printf("OK_006fedb0\n");
    return 0;
}