#include "engine/CCreatureAction_DragonFlight.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_DragonFlight_Methods : CCreatureAction_DragonFlight {
    unsigned char M();
};
unsigned char CCreatureAction_DragonFlight_Methods::M(){ return this->f; }