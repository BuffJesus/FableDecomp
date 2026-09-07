#include "engine/CAIStateGroup_WorkSawingWood.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_WorkSawingWood_Methods : CAIStateGroup_WorkSawingWood {
    void M(int p0, int p1);
};
void CAIStateGroup_WorkSawingWood_Methods::M(int p0, int p1){ this->ActionPointIndex = 0u; this->TookActionPoint = 0; }