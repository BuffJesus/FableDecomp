#include "rebuild_abi.h"

// 0x004605e0 CSpecialAbilitiesThunderLightningStormDef::Copy
// Retail is a 5-byte incremental-link thunk: a single `jmp rel32` tail-call
// into the real Copy implementation (relocation-masked in parity).
// Modeled as a __fastcall accessor-style free function tail-calling the target.

struct CSpecialAbilitiesThunderLightningStormDef;

// The real implementation the thunk jumps to (address relocation-masked).
extern "C" void __fastcall CSpecialAbilitiesThunderLightningStormDef_Copy_impl(
    CSpecialAbilitiesThunderLightningStormDef* thisptr, void* src);

void __fastcall CSpecialAbilitiesThunderLightningStormDef_Copy(
    CSpecialAbilitiesThunderLightningStormDef* thisptr, void* src)
{
    // Tail call -> compiles to a single `jmp rel32`.
    CSpecialAbilitiesThunderLightningStormDef_Copy_impl(thisptr, src);
}