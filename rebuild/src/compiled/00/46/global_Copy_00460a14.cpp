#include "rebuild_abi.h"

/* Target 0x00460a14: CSpecialAbilitiesCreatureTintDef::Copy
 *
 * Retail bytes: e9 00 00 00 00  -> a single `jmp rel32` thunk.
 * The rel32 target is relocation-masked, so any extern jump target
 * byte-matches. This is a COMDAT-folded tail-call thunk: Copy simply
 * jumps to the shared implementation. Modeled as a __fastcall member
 * (this-only + one src param) that tail-calls the real copy routine.
 */

struct CSpecialAbilitiesCreatureTintDef;

/* The shared implementation this thunk jumps to (relocation-masked). */
extern void __fastcall CSpecialAbilitiesCreatureTintDef_CopyImpl(
    CSpecialAbilitiesCreatureTintDef* thisptr,
    CSpecialAbilitiesCreatureTintDef* src);

void __fastcall CSpecialAbilitiesCreatureTintDef_Copy(
    CSpecialAbilitiesCreatureTintDef* thisptr,
    CSpecialAbilitiesCreatureTintDef* src)
{
    /* tail-call -> compiles to `jmp rel32` */
    CSpecialAbilitiesCreatureTintDef_CopyImpl(thisptr, src);
}