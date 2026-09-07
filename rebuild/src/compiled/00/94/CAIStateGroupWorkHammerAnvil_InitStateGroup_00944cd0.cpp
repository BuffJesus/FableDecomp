#include "engine/CAIStateGroup_WorkHammerAnvil.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorld; struct CThingAICreature;
struct CAIStateGroup_WorkHammerAnvil_Methods : CAIStateGroup_WorkHammerAnvil {
    void Init(const CWorld&, CThingAICreature&);
};
void CAIStateGroup_WorkHammerAnvil_Methods::Init(const CWorld&, CThingAICreature&){ ActionPointIndex=0; TookActionPoint=0; NextFrameHammer=0; }