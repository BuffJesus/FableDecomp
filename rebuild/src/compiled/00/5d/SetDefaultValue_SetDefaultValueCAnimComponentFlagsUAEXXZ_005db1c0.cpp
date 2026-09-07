#include "engine/CAnimComponentFlags.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAnimComponentFlags_Methods : CAnimComponentFlags {
    void M();
};
void CAnimComponentFlags_Methods::M(){ this->Flags = 0u; }