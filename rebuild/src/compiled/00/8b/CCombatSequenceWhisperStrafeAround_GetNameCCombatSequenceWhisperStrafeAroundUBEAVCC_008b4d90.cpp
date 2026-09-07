// CCombatSequenceWhisperStrafeAround_Methods::GetName  (retail 0x008b4d90)
// Virtual method returning CCharString by value from a fixed string literal.

#include "engine/CCombatSequenceWhisperStrafeAround.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString {
    char* m_buf;
    // CCharString::CCharString(char const*, int)  @ 0x0099ebf0  (__fastcall)
    CCharString(const char* s, int n);
};

struct CCombatSequenceWhisperStrafeAround_Methods : CCombatSequenceWhisperStrafeAround {
    CCharString GetName() const;
};

CCharString CCombatSequenceWhisperStrafeAround_Methods::GetName() const
{
    return CCharString("CombatSequenceWhisperStrafeAround", -1);
}