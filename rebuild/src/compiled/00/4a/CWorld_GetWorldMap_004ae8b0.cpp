#include "engine/CWorld.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorldMap;
struct CWorld_Methods : CWorld {
    CWorldMap& G();
};
CWorldMap& CWorld_Methods::G(){ return *this->PWorldMap_ptr; }