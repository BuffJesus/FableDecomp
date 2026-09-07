// CCombatSequence_GenericLeader_Methods::GetName @ 008b3540
// __fastcall (modeled as member) returning CCharString by value:
// hidden retptr on stack, this in ecx, ret 4. Constructs a CCharString
// from the string literal at 0x12786d4 with length -1.

#include "engine/CCombatSequence_GenericLeader.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString {
    char* p;
    CCharString(const char* s, int len);
};

struct CCombatSequence_GenericLeader_Methods : CCombatSequence_GenericLeader {
    CCharString GetName() const;
};

CCharString CCombatSequence_GenericLeader_Methods::GetName() const
{
    return CCharString((const char*)0x12786d4, -1);
}