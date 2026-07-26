#include "rebuild_abi.h"

// 0x0046244c CMeleeCombatAbilityDef::Copy (5-byte thunk: jmp rel32)
// Retail is a single tail-call jump into the real implementation.
// Modeled as a free __fastcall (this-only member accessor style); the
// relocation-masked jmp target is an extern callee.

struct CMeleeCombatAbilityDef;

// The real copy implementation this thunk tail-jumps to.
extern void __fastcall CMeleeCombatAbilityDef_Copy_impl(CMeleeCombatAbilityDef* thisptr, void* other);

void __fastcall CMeleeCombatAbilityDef_Copy(CMeleeCombatAbilityDef* thisptr, void* other)
{
    CMeleeCombatAbilityDef_Copy_impl(thisptr, other);
}