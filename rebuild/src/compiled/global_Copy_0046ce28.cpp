#include "rebuild_abi.h"

// 0x0046ce28 CPlayerGuiDef::Copy -- 5-byte jmp-thunk (tail-call to real impl).
// Retail: e9 00000000 => jmp rel32 (target relocation-masked).
// Model the destination as an extern free function; the thunk is a plain tail call.

struct CPlayerGuiDef;

// Real implementation target (address relocation-masked in parity).
extern void __fastcall CPlayerGuiDef_Copy_impl(CPlayerGuiDef* thisptr, void* other);

// The thunk itself: __fastcall(this, other) tail-calls the impl, producing a
// single `jmp rel32`.
void __fastcall CPlayerGuiDef_Copy(CPlayerGuiDef* thisptr, void* other)
{
    CPlayerGuiDef_Copy_impl(thisptr, other);
}