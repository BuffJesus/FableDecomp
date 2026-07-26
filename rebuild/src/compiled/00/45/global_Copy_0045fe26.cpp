#include "rebuild_abi.h"

/* CSpecialAbilitiesEnflameDef::Copy @ 0x0045fe26
 * Retail body is a single 5-byte tail-call jmp (e9 rel32) to an
 * identical/folded implementation. The rel32 target is
 * relocation-masked in parity, so a plausible extern forward target
 * reproduces the byte pattern exactly. */

struct CSpecialAbilitiesEnflameDef;

/* Folded/forwarded target (relocation-masked). Same signature so the
 * tail call compiles to a plain jmp with no register shuffling. */
extern void __fastcall CSpecialAbilitiesEnflameDef_Copy_impl(
    CSpecialAbilitiesEnflameDef* thisptr, void* other);

void __fastcall CSpecialAbilitiesEnflameDef_Copy(
    CSpecialAbilitiesEnflameDef* thisptr, void* other)
{
    CSpecialAbilitiesEnflameDef_Copy_impl(thisptr, other);
}