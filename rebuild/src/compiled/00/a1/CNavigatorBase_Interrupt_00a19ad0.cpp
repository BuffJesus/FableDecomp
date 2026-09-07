#include "engine/CNavigatorBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CNavigatorBase_Methods : CNavigatorBase {
    void M();
};
void CNavigatorBase_Methods::M(){ this->Interrupted = 1; }