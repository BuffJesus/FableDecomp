#include "rebuild_abi.h"

// 0x0045fabc  ?Copy@CSpecialAbilitiesBulletTimeDef@@UAEXPBVCDefClassBase@@@Z
// 5-byte thunk: a single `jmp rel32` tail-call into the real Copy implementation.
// Modeled as a free __fastcall(this-only + one arg) that tail-calls an extern impl;
// VC7.1 compiles the tail-call to `e9 <rel32>` (rel32 relocation-masked in parity).

struct CSpecialAbilitiesBulletTimeDef;
struct CDefClassBase;

// The real implementation this thunk jumps to (address relocation-masked).
extern "C" void __fastcall CSpecialAbilitiesBulletTimeDef_Copy_impl(
    CSpecialAbilitiesBulletTimeDef* thisptr, const CDefClassBase* src);

void __fastcall CSpecialAbilitiesBulletTimeDef_Copy(
    CSpecialAbilitiesBulletTimeDef* thisptr, const CDefClassBase* src)
{
    // tail-call -> jmp rel32
    CSpecialAbilitiesBulletTimeDef_Copy_impl(thisptr, src);
}