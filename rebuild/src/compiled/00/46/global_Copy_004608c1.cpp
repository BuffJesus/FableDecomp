#include "rebuild_abi.h"

/* CSpecialAbilitiesUnholyPowerDef::Copy @ 0x004608c1
 * Retail body is a single 5-byte `jmp rel32` (relocation-masked tail-call).
 * Modeled as a this-only member (two params: this + source) that tail-calls
 * the shared Copy implementation. Written __fastcall(this, src) so the byte
 * pattern is a bare tail jump with no extra prologue/epilogue.
 */

struct CSpecialAbilitiesUnholyPowerDef;

/* The tail-call target (address relocation-masked in parity). */
extern void __fastcall CSpecialAbilitiesUnholyPowerDef_Copy_impl(
    CSpecialAbilitiesUnholyPowerDef* thisptr, void* src);

void __fastcall CSpecialAbilitiesUnholyPowerDef_Copy(
    CSpecialAbilitiesUnholyPowerDef* thisptr, void* src)
{
    /* tail call -> emits `jmp rel32` */
    CSpecialAbilitiesUnholyPowerDef_Copy_impl(thisptr, src);
}