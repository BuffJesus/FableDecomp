#include "engine/CIEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CIEngine_Methods : CIEngine {
    void M();
};
void CIEngine_Methods::M(){ this->Active = 1u; }