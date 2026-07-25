#include "rebuild_abi.h"

// 0x0045c549 CSoundThemeDef::Copy — a 5-byte incremental-linker style tail-call thunk:
//   e9 <rel32>   ->   jmp <target>
// The target is relocation-masked, so any plausible extern callee byte-matches.
// Model the this-only member as a free __fastcall(objptr, arg).

struct CSoundThemeDef;

// The real implementation this thunk tail-jumps to (relocation-masked target).
extern void __fastcall CSoundThemeDef_Copy_impl(CSoundThemeDef* thisptr, void* src);

void __fastcall CSoundThemeDef_Copy(CSoundThemeDef* thisptr, void* src)
{
    // Pure tail call -> compiles to `jmp CSoundThemeDef_Copy_impl` (e9 rel32).
    CSoundThemeDef_Copy_impl(thisptr, src);
}