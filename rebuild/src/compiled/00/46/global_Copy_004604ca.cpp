#include "rebuild_abi.h"

// CSpecialAbilitiesBurntEffectDef::Copy @ 0x004604ca
// Retail is a 5-byte jump thunk: e9 00000000 -> a single `jmp rel32`
// (relocation masked). This is a tail-call to the real implementation.
// Model the this-only member as a FREE __fastcall(objptr, src) so the
// compiler emits a plain tail-call jmp.

struct CSpecialAbilitiesBurntEffectDef;

// The real Copy implementation (relocation-masked call/jmp target).
extern void __fastcall CSpecialAbilitiesBurntEffectDef_Copy_impl(
    CSpecialAbilitiesBurntEffectDef* thisptr, void* src);

void __fastcall CSpecialAbilitiesBurntEffectDef_Copy(
    CSpecialAbilitiesBurntEffectDef* thisptr, void* src)
{
    // Tail call -> emits `jmp rel32`.
    CSpecialAbilitiesBurntEffectDef_Copy_impl(thisptr, src);
}