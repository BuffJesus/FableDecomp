#include <string.h>

struct CWorld;
struct CThingAICreature;

struct CAIStateGroup_OpinionReactionRoot {
    char pad[0x18];
    int field_18;
    unsigned char states[79];
};

void __fastcall InitStateGroup(CAIStateGroup_OpinionReactionRoot* self, int edx_dummy,
                               const CWorld& world, CThingAICreature& creature)
{
    unsigned char* d = self->states;
    self->field_18 = 0;
    for (int i = 0; i < 79; ++i) d[i] = 1;
}