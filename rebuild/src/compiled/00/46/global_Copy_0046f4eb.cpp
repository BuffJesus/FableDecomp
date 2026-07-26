#include "rebuild_abi.h"

// 0x0046f4eb  CSkyDef::Copy  (5-byte incremental-link jump thunk)
// Retail body is a single `jmp rel32` tail-call to the real implementation.
// Model the this-only member as a free __fastcall(thisptr, arg) and tail-call
// the (relocation-masked) real target so VC7.1 emits the same `e9 xx xx xx xx`.

struct CSkyDef;

// Real implementation target (relocation-masked in parity).
extern void __fastcall CSkyDef_Copy_impl(CSkyDef* thisptr, const CSkyDef* src);

void __fastcall CSkyDef_Copy(CSkyDef* thisptr, const CSkyDef* src)
{
    CSkyDef_Copy_impl(thisptr, src);
}