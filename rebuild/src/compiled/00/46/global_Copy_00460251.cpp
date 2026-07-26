#include "rebuild_abi.h"

// 0x00460251 CSpecialAbilitiesFireballSpellDef::Copy
// Retail body is a single 5-byte `jmp rel32` tail-call thunk (relocation-masked).
// Model as a __fastcall(this, src) that tail-calls the real Copy implementation.
// Modeled member of the target type.
struct CSpecialAbilitiesFireballSpellDef;

// The tail-call target (relocation-masked; a plausible extern decl suffices).
extern void __fastcall CSpecialAbilitiesFireballSpellDef_Copy_impl(
    CSpecialAbilitiesFireballSpellDef* thisptr, void* src);

void __fastcall CSpecialAbilitiesFireballSpellDef_Copy(
    CSpecialAbilitiesFireballSpellDef* thisptr, void* src)
{
    // Tail call -> compiles to `jmp rel32`.
    CSpecialAbilitiesFireballSpellDef_Copy_impl(thisptr, src);
}