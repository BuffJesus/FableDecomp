#include "rebuild_abi.h"

// 0x0045fd4e CSpecialAbilitiesForcePushDef::Copy (5-byte tail-call thunk: jmp rel32)
// __fastcall void Copy(this, src) -> tail-jumps to the real copy implementation.
// Modeled as __fastcall(objptr) free functions (byte-identical to __fastcall).

struct CSpecialAbilitiesForcePushDef;

// The real target of the tail call (relocation-masked; extern decl suffices).
extern void __fastcall CSpecialAbilitiesForcePushDef_Copy_impl(
    CSpecialAbilitiesForcePushDef* thisptr, void* /*edx*/, const void* src);

void __fastcall CSpecialAbilitiesForcePushDef_Copy(
    CSpecialAbilitiesForcePushDef* thisptr, void* edx, const void* src)
{
    // Single tail-call -> compiler emits `jmp rel32` (e9 xx xx xx xx).
    CSpecialAbilitiesForcePushDef_Copy_impl(thisptr, edx, src);
}