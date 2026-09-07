#include "engine/CWorld.h"  // retyped onto the PDB layout; byte parity re-verified
struct CThingManager;
struct CWorld_Methods : CWorld {
    CThingManager& G();
};
CThingManager& CWorld_Methods::G(){ return *this->PThingManager_ptr; }