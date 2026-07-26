#include "rebuild_abi.h"

/* 0x00464531 CThingPhysicalSwitchDef::Copy (5-byte jmp thunk)
 *
 * Retail bytes: e9 00 00 00 00  ==  jmp rel32
 * This is a tail-call thunk: the function body is a single jmp to the
 * real implementation. The rel32 target is relocation-masked in parity,
 * so any plausible extern callee produces byte-identical output.
 *
 * Modeled as a this-only member accessor -> free __fastcall(objptr, src).
 * The tail call compiles to `jmp rel32` (e9 xx xx xx xx) under VC7.1
 * when the callee has the identical signature (perfect argument
 * forwarding lets the compiler emit a jmp instead of call+ret).
 */

struct CThingPhysicalSwitchDef;

/* real Copy implementation (relocation-masked target of the jmp) */
extern void __fastcall CThingPhysicalSwitchDef_Copy_impl(
    CThingPhysicalSwitchDef* thisptr, void* src);

void __fastcall CThingPhysicalSwitchDef_Copy(
    CThingPhysicalSwitchDef* thisptr, void* src)
{
    CThingPhysicalSwitchDef_Copy_impl(thisptr, src);
}