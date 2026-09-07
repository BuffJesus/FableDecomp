#include "engine/CAnimComponentCombatRecoil.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAnimComponentCombatRecoil_Methods : CAnimComponentCombatRecoil {
    void M();
};
void CAnimComponentCombatRecoil_Methods::M(){ this->RecoilAnimIndex = 0xffffffffu; }