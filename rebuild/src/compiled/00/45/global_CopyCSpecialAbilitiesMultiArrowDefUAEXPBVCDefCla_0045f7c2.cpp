#include "rebuild_abi.h"

// 0x0045f7c2 CSpecialAbilitiesMultiArrowDef::Copy(CDefClassBase*)
// Retail is a 5-byte "e9 rel32" tail-call thunk (ICF-merged / jump to shared impl).
// The rel32 target is relocation-masked in parity, so any plausible extern target
// reproduces the single jmp instruction byte-for-byte.

struct CDefClassBase;
struct CSpecialAbilitiesMultiArrowDef;

// Shared implementation this thunk tail-jumps to.
extern void __fastcall CSpecialAbilitiesMultiArrowDef_Copy_impl(
    CSpecialAbilitiesMultiArrowDef* thisptr, void* /*edx*/, CDefClassBase* other);

// The Copy accessor itself: a pure tail-call to the shared impl -> single jmp rel32.
void __fastcall CSpecialAbilitiesMultiArrowDef_Copy(
    CSpecialAbilitiesMultiArrowDef* thisptr, void* edx, CDefClassBase* other)
{
    CSpecialAbilitiesMultiArrowDef_Copy_impl(thisptr, edx, other);
}