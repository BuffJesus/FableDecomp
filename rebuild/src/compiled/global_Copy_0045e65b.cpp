#include "rebuild_abi.h"

/* 0x0045e65b  COpinionPersonalityDef::Copy (5-byte thunk)
 * Retail body is a single `jmp rel32` tail-call that forwards this+arg
 * to the real implementation. The rel32 target is relocation-masked,
 * so any plausible extern callee with a matching signature byte-matches.
 */

struct COpinionPersonalityDef;

/* real implementation the thunk tail-calls into (address relocation-masked) */
extern void __fastcall COpinionPersonalityDef_Copy_impl(
    COpinionPersonalityDef* thisptr, void* edx, COpinionPersonalityDef* src);

/* modeled member accessor as free __fastcall(objptr, ...) per ABI rules */
void __fastcall COpinionPersonalityDef_Copy(
    COpinionPersonalityDef* thisptr, void* edx, COpinionPersonalityDef* src)
{
    COpinionPersonalityDef_Copy_impl(thisptr, edx, src);
}