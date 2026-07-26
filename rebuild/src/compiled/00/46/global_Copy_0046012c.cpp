#include "rebuild_abi.h"

// 0x0046012c  CSpecialAbilitiesHealLifeDef::Copy
// Retail body is a single 5-byte tail-call thunk: `e9 rel32` (jmp).
// The jump target is relocation-masked in parity, so any plausible
// extern callee with a matching __fastcall(this, src) signature that we
// tail-call produces the identical `jmp rel32` byte pattern.

struct CSpecialAbilitiesHealLifeDef;

// The forwarded-to implementation (e.g. a base-class / shared Copy).
// Declared extern; its address is relocation-masked at compare time.
extern void __fastcall CSpecialAbilitiesHealLifeDef_Copy_impl(
    CSpecialAbilitiesHealLifeDef* thisptr, void* src);

// Model the __fastcall accessor as a free __fastcall(this, ...).
// A pure tail-call compiles to a single `jmp rel32`.
void __fastcall CSpecialAbilitiesHealLifeDef_Copy(
    CSpecialAbilitiesHealLifeDef* thisptr, void* src)
{
    CSpecialAbilitiesHealLifeDef_Copy_impl(thisptr, src);
}