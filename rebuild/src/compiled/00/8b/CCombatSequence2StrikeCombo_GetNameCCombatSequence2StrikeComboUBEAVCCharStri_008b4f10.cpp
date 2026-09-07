// CCombatSequence2StrikeCombo::GetName  @ 008b4f10
// returns CCharString by value; hidden return buffer ptr is a stack arg.

#include "engine/CCombatSequence2StrikeCombo.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString {
    char* p;
    // CCharString::CCharString(char const*, int)  @ 0x99ebf0  (__fastcall)
    void ctor(const char* s, int n);
};


// __fastcall: self in ecx, hidden return-buffer ptr is first stack arg.
// Model as free __fastcall: (self=ecx, edx, retbuf).
extern "C" CCharString* __fastcall CCombatSequence2StrikeCombo_GetName(
    CCombatSequence2StrikeCombo* self, int edx, CCharString* ret)
{
    ret->ctor("2StrikeCombo", -1);
    return ret;
}