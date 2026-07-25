#include "rebuild_abi.h"

// 0x0045e555 CCombatDialogueDef::Copy — 5-byte tail-call thunk (jmp rel32).
// The rel32 target is relocation-masked; a tail call to any matching-signature
// extern reproduces the single `jmp` instruction byte-for-byte.

struct CCombatDialogueDef;

// The real copy implementation this thunk tail-jumps to.
extern void __fastcall CCombatDialogueDef_Copy_impl(CCombatDialogueDef* thisptr, void* src);

// Model the __fastcall member accessor as a free __fastcall(thisptr, ...).
void __fastcall CCombatDialogueDef_Copy(CCombatDialogueDef* thisptr, void* src)
{
    // Tail call -> compiles to a single `jmp rel32`.
    CCombatDialogueDef_Copy_impl(thisptr, src);
}