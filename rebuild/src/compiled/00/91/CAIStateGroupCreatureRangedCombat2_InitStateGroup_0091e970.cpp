#include "engine/CAIStateGroup_CreatureRangedCombat2.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld;
class CThingAICreature;

struct CAIStateGroup_CreatureRangedCombat2_Methods : CAIStateGroup_CreatureRangedCombat2 {
    void InitStateGroup(const CWorld &world, CThingAICreature &creature);
};

void CAIStateGroup_CreatureRangedCombat2_Methods::InitStateGroup(const CWorld &world, CThingAICreature &creature)
{
    this->AddedStrafe = 0;
}