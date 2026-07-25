#include "rebuild_abi.h"

// CEngineThemeDef::Copy — retail 0x004629f6
// Retail body is a single `jmp rel32` incremental-linking thunk:
//   0: e9 00 00 00 00   jmp <target>
// The rel32 is relocation-masked, so a tail-call to any extern with the
// same signature reproduces the exact 5-byte `jmp rel32` encoding.
// __fastcall(this, src) is modeled as __fastcall(objptr, src) per ABI rules.

struct CEngineThemeDef;

// Real implementation the thunk tail-jumps to (address relocation-masked).
extern void __fastcall CEngineThemeDef_Copy_impl(CEngineThemeDef* thisptr, CEngineThemeDef* src);

void __fastcall CEngineThemeDef_Copy(CEngineThemeDef* thisptr, CEngineThemeDef* src)
{
    // Tail call -> emits `jmp rel32` (5 bytes), matching retail `e9 00000000`.
    CEngineThemeDef_Copy_impl(thisptr, src);
}