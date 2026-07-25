#include "rebuild_abi.h"

// 0x00465eae CGlobalDef::Copy (5-byte jmp thunk / tail-call)
// Retail body is a single `jmp rel32` (e9 ........), i.e. a tail-call
// to the real copy implementation. rel32 target is relocation-masked
// in parity, so any extern tail-call target reproduces the bytes.

struct CGlobalDef;

// The real copy implementation this thunk tail-calls into.
extern void __fastcall CGlobalDef_Copy_impl(CGlobalDef* thisptr, CGlobalDef* other);

// Model the this-only member as a free __fastcall; a tail-call with the
// same argument registers compiles to a bare `jmp rel32`.
void __fastcall CGlobalDef_Copy(CGlobalDef* thisptr, CGlobalDef* other)
{
    CGlobalDef_Copy_impl(thisptr, other);
}