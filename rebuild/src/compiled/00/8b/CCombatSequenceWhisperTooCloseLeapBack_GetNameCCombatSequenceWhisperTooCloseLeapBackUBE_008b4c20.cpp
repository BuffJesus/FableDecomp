// CCombatSequenceWhisperTooCloseLeapBack_Methods::GetName -> returns CCharString by value.
#include "engine/CCombatSequenceWhisperTooCloseLeapBack.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString {
    char* p;
    // ctor at 0x99ebf0: (char const*, int)
    CCharString(const char* s, int n);
};

struct CCombatSequenceWhisperTooCloseLeapBack_Methods : CCombatSequenceWhisperTooCloseLeapBack {
    CCharString GetName() const;
};

CCharString CCombatSequenceWhisperTooCloseLeapBack_Methods::GetName() const
{
    return CCharString((const char*)0x1278d98, -1);
}