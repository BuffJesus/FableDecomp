#include "rebuild_abi.h"

// 0x00460518 CSpecialAbilitiesElectrocutedEffectDef::Copy
// Retail is a 5-byte tail-call thunk: `jmp rel32` (relocation-masked target).
// Model the this-only member as a free __fastcall and tail-call the real impl,
// which VC7.1 compiles to a single `jmp rel32`.

struct CSpecialAbilitiesElectrocutedEffectDef;

// The real copy implementation this thunk forwards to (address relocation-masked).
extern void __fastcall CSpecialAbilitiesElectrocutedEffectDef_Copy_impl(
    CSpecialAbilitiesElectrocutedEffectDef* thisptr, void* other);

void __fastcall CSpecialAbilitiesElectrocutedEffectDef_Copy(
    CSpecialAbilitiesElectrocutedEffectDef* thisptr, void* other)
{
    CSpecialAbilitiesElectrocutedEffectDef_Copy_impl(thisptr, other);
}