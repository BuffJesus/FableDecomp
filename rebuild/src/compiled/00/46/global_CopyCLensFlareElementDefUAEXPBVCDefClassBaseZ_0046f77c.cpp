#include "rebuild_abi.h"

// 0x0046f77c  ?Copy@CLensFlareElementDef@@UAEXPBVCDefClassBase@@@Z
// Retail body is a single 5-byte tail-call thunk:
//     jmp    rel32   ; (target relocation-masked in parity)
// i.e. CLensFlareElementDef::Copy forwards to a shared Copy implementation.

struct CDefClassBase;
struct CLensFlareElementDef;

// The real Copy implementation this thunk tail-calls into.
// Its address is relocation-masked, so a plausible extern decl is sufficient.
extern "C" void __fastcall CLensFlareElementDef_Copy_impl(CLensFlareElementDef* thisptr, const CDefClassBase* other);

// Model the __fastcall member as a free __fastcall(objptr) function.
// A single tail-call compiles to `jmp rel32`, byte-identical to the retail thunk.
void __fastcall CLensFlareElementDef_Copy(CLensFlareElementDef* thisptr, const CDefClassBase* other)
{
    CLensFlareElementDef_Copy_impl(thisptr, other);
}