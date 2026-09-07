// Clear the two byte flags at this+0x8 / this+0x9. __fastcall this=ecx.
#include "engine/CAnimComponentCombatMisc.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
struct CAnimComponentCombatMisc_Methods : CAnimComponentCombatMisc {
    void Clear();
};
#pragma pack(pop)
void CAnimComponentCombatMisc_Methods::Clear() { this->MeleeFlourish = false; this->MeleeKnockdown = false; }