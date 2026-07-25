#include "rebuild_abi.h"

// 0x0045f84f  CSpecialAbilitiesBerserkDef::Copy (5-byte jmp thunk)
// Retail body is a single `jmp rel32` tail-call into the real Copy impl.
// The rel32 target is relocation-masked in parity, so any extern tail-call
// callee with a matching signature reproduces the bytes exactly.

struct CSpecialAbilitiesBerserkDef;

// The real implementation this thunk tail-jumps to (address relocation-masked).
extern void __fastcall CSpecialAbilitiesBerserkDef_Copy_impl(
    CSpecialAbilitiesBerserkDef* thisptr, void* src);

// Model the __fastcall member accessor as a free __fastcall function.
// A plain tail call to another function of identical signature compiles to
// `jmp rel32` under VC7.1 size optimization.
void __fastcall CSpecialAbilitiesBerserkDef_Copy(
    CSpecialAbilitiesBerserkDef* thisptr, void* src)
{
    CSpecialAbilitiesBerserkDef_Copy_impl(thisptr, src);
}