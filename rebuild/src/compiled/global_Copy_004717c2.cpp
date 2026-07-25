#include "rebuild_abi.h"

/* 0x004717c2 CAppearanceDef::Copy
 * Retail body is a single `jmp rel32` (e9 00000000) tail-call thunk:
 * the whole function forwards to the real Copy implementation.
 * Modeled as a free __fastcall(thisptr, src) that tail-calls the
 * relocation-masked extern target. The rel32 operand is masked in
 * parity, so any plausible callee address byte-matches.
 */

struct CAppearanceDef;

/* real implementation the thunk forwards to (relocation-masked target) */
extern void __fastcall CAppearanceDef_Copy_impl(CAppearanceDef* thisptr, CAppearanceDef* src);

void __fastcall CAppearanceDef_Copy(CAppearanceDef* thisptr, CAppearanceDef* src)
{
    /* tail-call -> compiles to a single `jmp rel32` */
    CAppearanceDef_Copy_impl(thisptr, src);
}