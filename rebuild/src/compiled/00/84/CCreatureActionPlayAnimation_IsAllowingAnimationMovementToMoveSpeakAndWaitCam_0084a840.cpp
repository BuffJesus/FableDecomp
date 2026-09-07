#include "engine/CCreatureAction_PlayAnimation.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_PlayAnimation_Methods : CCreatureAction_PlayAnimation {
    unsigned char M();
};
unsigned char CCreatureAction_PlayAnimation_Methods::M(){ return this->f_3; }