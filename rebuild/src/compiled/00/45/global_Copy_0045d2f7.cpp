#include "rebuild_abi.h"

/* 0x0045d2f7  CFactionDef::Copy (5-byte jmp thunk)
 * Retail: e9 00 00 00 00  -> jmp rel32 (relocation-masked).
 * This is an incremental-linker / tail-call thunk that jumps to the real
 * CFactionDef::Copy implementation. Modeled as a member (this-only + 1 arg)
 * via free __fastcall, tail-calling the real target so VC7.1 emits a single
 * `jmp rel32`.
 */

struct CFactionDef;

/* Real target of the thunk (relocation-masked; extern decl is sufficient). */
extern void __fastcall CFactionDef_Copy_impl(CFactionDef* thisptr, void* src);

void __fastcall CFactionDef_Copy_thunk(CFactionDef* thisptr, void* src)
{
    /* tail call -> compiles to a single jmp rel32 */
    CFactionDef_Copy_impl(thisptr, src);
}