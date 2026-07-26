#include "rebuild_abi.h"

/* 0x00462e28  CPlayerInventoryDef::Copy
 * Retail body is a single 5-byte `jmp rel32` thunk (tail call). The rel32
 * target is relocation-masked in parity, so any plausible extern callee with
 * a matching signature reproduces the exact `E9 xx xx xx xx` byte pattern.
 *
 * Signature per manifest: void __fastcall(this, arg) -> model as __fastcall
 * free function taking the object pointer (byte-identical to __fastcall).
 */

struct CPlayerInventoryDef;

/* The real (non-thunk) implementation this thunk tail-calls into. */
extern void __fastcall CPlayerInventoryDef_Copy_impl(CPlayerInventoryDef* thisptr, void* src);

void __fastcall CPlayerInventoryDef_Copy(CPlayerInventoryDef* thisptr, void* src)
{
    CPlayerInventoryDef_Copy_impl(thisptr, src);
}