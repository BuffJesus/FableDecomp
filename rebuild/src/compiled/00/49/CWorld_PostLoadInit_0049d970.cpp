#include "engine/CWorld.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorld_Methods : CWorld {
    void PostLoadInit();
};
void CWorld_Methods::PostLoadInit(){ this->JustLoaded = 1; }