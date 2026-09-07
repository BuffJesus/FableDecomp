#include "engine/CEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngine_Methods : CEngine {
    void M();
};
void CEngine_Methods::M(){ this->f = 1u; }