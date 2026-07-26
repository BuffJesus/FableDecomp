#include "rebuild_abi.h"

// 0x00460452 CSpecialAbilitiesSummonSpellDef::Copy
// Retail is a 5-byte jump thunk: e9 rel32 (a tail-call to the real Copy impl).
// rel32 is relocation-masked, so any tail-call producing a single `jmp` matches.

struct CSpecialAbilitiesSummonSpellDef;

// The real implementation the thunk tail-calls (address relocation-masked).
extern void __fastcall CSpecialAbilitiesSummonSpellDef_Copy_impl(
    CSpecialAbilitiesSummonSpellDef* thisptr, void* src);

void __fastcall CSpecialAbilitiesSummonSpellDef_Copy(
    CSpecialAbilitiesSummonSpellDef* thisptr, void* src)
{
    // Tail call -> VC7.1 emits `jmp CSpecialAbilitiesSummonSpellDef_Copy_impl`
    CSpecialAbilitiesSummonSpellDef_Copy_impl(thisptr, src);
}