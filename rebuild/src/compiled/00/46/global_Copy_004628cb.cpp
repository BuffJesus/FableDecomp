#include "rebuild_abi.h"

// 0x004628cb  CPlayerMovementDef::Copy  (5-byte incremental-link jump thunk)
// Retail body is a single `jmp rel32` (E9 ..) whose target is relocation-masked.
// Model as a tail-call: the compiler emits exactly one `jmp rel32` for the
// forwarded call, matching the retail thunk byte-for-byte (post-mask).

struct CPlayerMovementDef;

// Real target of the thunk (address relocation-masked in parity).
extern void __fastcall CPlayerMovementDef_Copy_impl(CPlayerMovementDef* thisptr, CPlayerMovementDef* src);

void __fastcall CPlayerMovementDef_Copy(CPlayerMovementDef* thisptr, CPlayerMovementDef* src)
{
    CPlayerMovementDef_Copy_impl(thisptr, src);
}