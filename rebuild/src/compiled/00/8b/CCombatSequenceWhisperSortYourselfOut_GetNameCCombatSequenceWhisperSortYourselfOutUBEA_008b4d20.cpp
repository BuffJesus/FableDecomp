// CCombatSequenceWhisperSortYourselfOut_Methods::GetName
// retail 0x008b4d20 : returns CCharString by value from a fixed string literal.
// The hidden struct-return buffer is constructed in place via the CCharString
// ctor at 0x99ebf0 (const char*, int len = -1). 'this' (ecx) is unused.

#include "engine/CCombatSequenceWhisperSortYourselfOut.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString
{
    void* buf;
    CCharString(const char* s, int len);   // ctor @ 0x0099ebf0 (not defined here)
};

struct CCombatSequenceWhisperSortYourselfOut_Methods : CCombatSequenceWhisperSortYourselfOut {
    CCharString GetName() const;
};

CCharString CCombatSequenceWhisperSortYourselfOut_Methods::GetName() const
{
    return CCharString((const char*)0x1278de4, -1);
}