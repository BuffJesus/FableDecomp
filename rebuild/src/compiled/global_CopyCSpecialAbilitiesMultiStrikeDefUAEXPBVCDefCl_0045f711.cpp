#include "rebuild_abi.h"

// 0045f711 ?Copy@CSpecialAbilitiesMultiStrikeDef@@UAEXPBVCDefClassBase@@@Z
// Retail body is a single `jmp rel32` (5-byte thunk / tail-call) to a shared
// Copy implementation with the identical (this, const CDefClassBase*) shape.
// The jmp target is relocation-masked, so any plausible extern of the same
// signature produces byte-identical output (just `jmp rel32`).

struct CDefClassBase;
struct CSpecialAbilitiesMultiStrikeDef;

// Shared implementation the thunk tail-calls into.
extern void __fastcall CSpecialAbilitiesMultiStrikeDef_Copy_impl(
    CSpecialAbilitiesMultiStrikeDef* thisptr, const CDefClassBase* src);

void __fastcall CSpecialAbilitiesMultiStrikeDef_Copy(
    CSpecialAbilitiesMultiStrikeDef* thisptr, const CDefClassBase* src)
{
    CSpecialAbilitiesMultiStrikeDef_Copy_impl(thisptr, src);
}