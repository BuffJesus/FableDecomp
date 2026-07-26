#include "rebuild_abi.h"

// 0x004600c6 CSpecialAbilitiesDrainLifeDef::Copy
// Retail is a 5-byte thunk: a single `jmp rel32` tail-call into the base-class
// Copy implementation. The jmp target is relocation-masked, so any plausible
// extern tail-callee reproduces the bytes.

struct CSpecialAbilitiesDrainLifeDef;

// Base-class Copy that this def forwards to (address relocation-masked).
extern "C" void __fastcall CSpecialAbilitiesDrainLifeDef_Copy_base(
    CSpecialAbilitiesDrainLifeDef* thisptr, void* src);

// Model the __fastcall accessor as a free __fastcall(objptr, ...) — byte-identical.
void __fastcall CSpecialAbilitiesDrainLifeDef_Copy(
    CSpecialAbilitiesDrainLifeDef* thisptr, void* src)
{
    // Tail call -> compiler emits `jmp rel32`.
    CSpecialAbilitiesDrainLifeDef_Copy_base(thisptr, src);
}