// CCombatSequence_BanditKingAdvance_Methods::GetName -- returns CCharString by value.
// Retail: builds a CCharString from a string literal (len = -1) into the
// hidden return buffer and returns it. 'this' is unused.

#include "engine/CCombatSequence_BanditKingAdvance.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString {
    char* m_ptr;
    CCharString(const char* s, int len);
};

struct CCombatSequence_BanditKingAdvance_Methods : CCombatSequence_BanditKingAdvance {
    CCharString GetName() const;
};

CCharString CCombatSequence_BanditKingAdvance_Methods::GetName() const {
    return CCharString("BanditKingAdvance", -1);
}