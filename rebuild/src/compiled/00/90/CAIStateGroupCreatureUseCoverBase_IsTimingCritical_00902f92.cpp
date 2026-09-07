#include "engine/CAIStateGroup_CreatureUseCoverBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_CreatureUseCoverBase_Methods : CAIStateGroup_CreatureUseCoverBase {
    unsigned char M();
};
unsigned char CAIStateGroup_CreatureUseCoverBase_Methods::M(){ return this->TimingCritical; }