#include "rebuild_abi.h"

/* COpinionDeedEffectsDef::Copy @ 0x00471a72
   Retail body is a single `jmp rel32` (5 bytes: e9 00 00 00 00) — a tail-call
   thunk that forwards this + one argument to another routine. The rel32 target
   is relocation-masked, so any plausible extern callee byte-matches.

   Modeled as a free __fastcall(objptr, arg) per the ABI rules for this-only
   members; the tail call reuses the incoming ecx/edx unchanged, which the
   compiler emits as a plain jmp. */

struct COpinionDeedEffectsDef;

/* Forwarding target (relocation-masked). Same calling convention/signature so
   the incoming this + arg pass straight through as a tail jmp. */
extern void __fastcall COpinionDeedEffectsDef_Copy_impl(COpinionDeedEffectsDef* thisptr, void* src);

void __fastcall COpinionDeedEffectsDef_Copy(COpinionDeedEffectsDef* thisptr, void* src)
{
    COpinionDeedEffectsDef_Copy_impl(thisptr, src);
}