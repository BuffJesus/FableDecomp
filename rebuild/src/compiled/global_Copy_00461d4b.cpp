#include "rebuild_abi.h"

// CCombatSequenceDef::Copy @ 0x00461d4b
// Retail is a 5-byte tail-call thunk: `e9 00 00 00 00` == jmp rel32.
// The jmp target is relocation-masked, so any tail-call to an extern
// function of matching signature reproduces the single `jmp rel32`.
//
// Signature (manifest): __fastcall void Copy(this, arg) -> 2 params.
// Model this-only member as a FREE __fastcall(thisptr, arg) so the
// accessor is byte-identical to __fastcall.

struct CCombatSequenceDef;

// Relocation-masked tail-call target (the real Copy implementation body).
extern void __fastcall CCombatSequenceDef_Copy_impl(CCombatSequenceDef* thisptr, void* other);

void __fastcall CCombatSequenceDef_Copy(CCombatSequenceDef* thisptr, void* other)
{
    // Pure tail call -> compiles to a single `jmp rel32` (the 5-byte thunk).
    CCombatSequenceDef_Copy_impl(thisptr, other);
}