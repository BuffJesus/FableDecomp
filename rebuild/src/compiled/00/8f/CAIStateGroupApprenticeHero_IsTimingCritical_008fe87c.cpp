#include "engine/CAIStateGroup_ApprenticeHero.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_ApprenticeHero_Methods : CAIStateGroup_ApprenticeHero {
    unsigned char M();
};
unsigned char CAIStateGroup_ApprenticeHero_Methods::M(){ return this->Critical; }