#include "engine/CCreatureAction_BlockRespond.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCreatureAction_BlockRespond_Methods : CCreatureAction_BlockRespond {
    unsigned char M();
};
unsigned char CCreatureAction_BlockRespond_Methods::M(){ return this->f; }