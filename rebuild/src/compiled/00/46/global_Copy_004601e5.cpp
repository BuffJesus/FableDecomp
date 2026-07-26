#include "rebuild_abi.h"

// 0x004601e5 CSpecialAbilitiesTurncoatSpellDef::Copy
// Retail is a 5-byte tail-call thunk: `jmp rel32` into the real Copy impl
// (relocation-masked target). Modeled as a member accessor via __fastcall
// (this-only + one arg), tail-calling the delegate so VC7.1 emits a single jmp.

struct CSpecialAbilitiesTurncoatSpellDef;

// The real Copy implementation this thunk jumps to (target reloc-masked).
extern void __fastcall CSpecialAbilitiesTurncoatSpellDef_Copy_impl(
    CSpecialAbilitiesTurncoatSpellDef* thisptr, void* src);

void __fastcall CSpecialAbilitiesTurncoatSpellDef_Copy(
    CSpecialAbilitiesTurncoatSpellDef* thisptr, void* src)
{
    CSpecialAbilitiesTurncoatSpellDef_Copy_impl(thisptr, src);
}