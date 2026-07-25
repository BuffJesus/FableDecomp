#include "rebuild_abi.h"

// 0x0045f630 CSpecialAbilitiesBattleChargeDef::Copy
// Retail is a 5-byte thunk: a single `jmp rel32` (tail-call) into the real
// Copy implementation. The jmp rel32 target is relocation-masked in parity,
// so any tail-call to an extern member-shaped function reproduces the bytes.

struct CSpecialAbilitiesBattleChargeDef;

// The real implementation this thunk tail-jumps to (address relocation-masked).
extern void __fastcall CSpecialAbilitiesBattleChargeDef_Copy_impl(
    CSpecialAbilitiesBattleChargeDef* thisptr, void* src);

void __fastcall CSpecialAbilitiesBattleChargeDef_Copy(
    CSpecialAbilitiesBattleChargeDef* thisptr, void* src)
{
    // Tail-call -> compiles to `jmp rel32`.
    CSpecialAbilitiesBattleChargeDef_Copy_impl(thisptr, src);
}