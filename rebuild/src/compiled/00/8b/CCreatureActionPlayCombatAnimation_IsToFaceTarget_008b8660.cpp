#include "engine/CCreatureAction_PlayCombatAnimation.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_PlayCombatAnimation_Methods : CCreatureAction_PlayCombatAnimation {
    unsigned char M();
};
unsigned char CCreatureAction_PlayCombatAnimation_Methods::M(){ return this->f; }