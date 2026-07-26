#include "rebuild_abi.h"

// 0x00462b10  CBrainDef::Copy  (5-byte jmp thunk)
// Retail body is a single `jmp rel32` (relocation-masked) — a tail-call
// forwarding this-only Copy to its real implementation. Model the member as
// a free __fastcall(thisptr) and tail-call an extern target so the compiler
// emits exactly one `jmp rel32`.

struct CBrainDef;

// Real implementation the thunk forwards to (address relocation-masked).
extern void __fastcall CBrainDef_Copy_impl(CBrainDef* thisptr, void* src);

void __fastcall CBrainDef_Copy(CBrainDef* thisptr, void* src)
{
    CBrainDef_Copy_impl(thisptr, src);
}