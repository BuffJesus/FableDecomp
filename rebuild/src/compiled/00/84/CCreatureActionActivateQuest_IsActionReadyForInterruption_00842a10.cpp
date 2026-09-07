#include "engine/CCreatureAction_ActivateQuest.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_ActivateQuest_Methods : CCreatureAction_ActivateQuest {
    unsigned char M();
};
unsigned char CCreatureAction_ActivateQuest_Methods::M(){ return this->f; }