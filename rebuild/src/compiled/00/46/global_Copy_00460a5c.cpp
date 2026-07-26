#include "rebuild_abi.h"

/* 0x00460a5c  CSoundDef::Copy -- 5-byte incremental-link thunk: jmp rel32.
   The rel32 target is relocation-masked, so any single tail-call to an
   extern __fastcall(objptr,...) compiles to the identical `jmp rel32`. */

struct CSoundDef;

/* real implementation the thunk tail-jumps to (relocation-masked target) */
extern void __fastcall CSoundDef_Copy_impl(CSoundDef* thisptr, void* src);

void __fastcall CSoundDef_Copy(CSoundDef* thisptr, void* src)
{
    /* pure tail call -> single `jmp rel32` (5 bytes), no prologue/epilogue */
    CSoundDef_Copy_impl(thisptr, src);
}