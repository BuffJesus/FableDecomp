#include "rebuild_abi.h"

/* 0x0046133f  CArmourDef::Copy (5 bytes)
 * Retail body is a single relocation-masked `jmp rel32` (e9 00000000):
 * a thunk that tail-calls the real copy implementation. Written as a
 * free __fastcall(thisptr, other) so the compiler emits an identical
 * tail-call jmp. Callee address is relocation-masked in parity. */

struct CArmourDef;

/* real implementation this thunk tail-calls (relocation-masked target) */
extern void __fastcall CArmourDef_Copy_impl_0046133f(CArmourDef* thisptr, CArmourDef* other);

void __fastcall CArmourDef_Copy_0046133f(CArmourDef* thisptr, CArmourDef* other)
{
    CArmourDef_Copy_impl_0046133f(thisptr, other);
}