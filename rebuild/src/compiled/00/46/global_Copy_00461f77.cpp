#include "rebuild_abi.h"

// TARGET 0x00461f77  CHeroCombatDef::Copy  (5-byte jmp thunk)
// Retail bytes: e9 <rel32>  -> a single tail-call (jmp rel32), target relocation-masked.
// Modeled as a this-only member (__fastcall(objptr, arg)) that tail-calls the real
// implementation. VC7.1 emits `jmp rel32` for a tail call whose result/void matches,
// which byte-matches (rel32 masked in parity).

struct CHeroCombatDef;

// The real implementation this thunk forwards to (relocation-masked extern).
extern "C" void __fastcall CHeroCombatDef_Copy_impl(CHeroCombatDef* thisptr, CHeroCombatDef* src);

void __fastcall CHeroCombatDef_Copy(CHeroCombatDef* thisptr, CHeroCombatDef* src)
{
    CHeroCombatDef_Copy_impl(thisptr, src);
}