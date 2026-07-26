#include "rebuild_abi.h"

// 0x0045e182 COpinionDeedMaskDef::Copy — 5-byte incremental-linker thunk:
//   e9 00000000  jmp rel32  (tail-call to the real Copy implementation)
// The rel32 target is relocation-masked, so any tail-call to an extern
// produces the identical `jmp rel32` byte pattern.

struct COpinionDeedMaskDef;

// The real target of the thunk (address relocation-masked in parity).
extern void __fastcall COpinionDeedMaskDef_Copy_impl(COpinionDeedMaskDef* thisptr, void* other);

// Thunk: a bare tail-call compiles to `jmp rel32`.
void __fastcall COpinionDeedMaskDef_Copy(COpinionDeedMaskDef* thisptr, void* other)
{
    COpinionDeedMaskDef_Copy_impl(thisptr, other);
}