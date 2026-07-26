#include "rebuild_abi.h"

// 0x0045dd00  COpinionReactionMaskDef::Copy  (__fastcall, void, 2 params)
// Retail is a 5-byte jump thunk: `e9 00 00 00 00` == jmp rel32.
// This is a tail-call thunk to the real implementation; the rel32 target
// is relocation-masked in parity, so any tail call to a plausible extern
// with the matching signature reproduces the exact bytes (jmp rel32).

struct COpinionReactionMaskDef;

// The real implementation this thunk tail-jumps to (relocation-masked target).
extern void __fastcall COpinionReactionMaskDef_Copy_impl(COpinionReactionMaskDef* thisptr, void* src);

// Modeled as a free __fastcall taking the this-pointer (byte-identical to __fastcall).
void __fastcall COpinionReactionMaskDef_Copy(COpinionReactionMaskDef* thisptr, void* src)
{
    // Tail call -> emits a single `jmp rel32` (the 5-byte thunk).
    COpinionReactionMaskDef_Copy_impl(thisptr, src);
}