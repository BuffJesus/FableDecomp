#include "rebuild_abi.h"

/* 0x004625f8  CMeleeCombatKnockdownEffects::Copy
 * Retail body is a single relocation-masked tail JMP (jmp rel32) to the
 * real copy implementation. Modeled as a __fastcall free function (this-only
 * member accessor) that tail-calls the extern target so the compiler emits a
 * lone `jmp rel32` (E9 xx xx xx xx). The rel32 operand is relocation-masked,
 * so the exact target need not match.
 */

struct CMeleeCombatKnockdownEffects;

/* extern target of the tail jump (relocation-masked address/operand) */
extern void __fastcall CMeleeCombatKnockdownEffects_Copy_impl(
    CMeleeCombatKnockdownEffects* thisptr, void* src);

void __fastcall CMeleeCombatKnockdownEffects_Copy(
    CMeleeCombatKnockdownEffects* thisptr, void* src)
{
    CMeleeCombatKnockdownEffects_Copy_impl(thisptr, src);
}