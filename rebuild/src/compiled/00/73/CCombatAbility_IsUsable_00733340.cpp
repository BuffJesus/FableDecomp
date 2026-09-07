#include "engine/CCombatAbility.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCombatAbility_Methods : CCombatAbility {
    char M();
};
char CCombatAbility_Methods::M(){ if(this->ActiveTimer >= 0) return 1; return this->RepeatTimer == -1; }