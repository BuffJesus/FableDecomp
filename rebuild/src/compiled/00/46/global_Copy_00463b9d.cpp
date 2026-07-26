#include "rebuild_abi.h"

// 0x00463b9d CEntitySoundDef::Copy -- a 5-byte jmp rel32 thunk (tail-call).
// The whole retail body is a single `jmp rel32` to the real Copy implementation;
// rel32 is relocation-masked in parity, so any extern target yields identical bytes.

struct CEntitySoundDef;

// Real implementation the thunk tail-jumps to (relocation-masked target).
extern void __fastcall CEntitySoundDef_Copy_impl(CEntitySoundDef* thisptr, CEntitySoundDef* src);

// The thunk: models `jmp rel32` via a tail-call with matching signature.
void __fastcall CEntitySoundDef_Copy(CEntitySoundDef* thisptr, CEntitySoundDef* src)
{
    CEntitySoundDef_Copy_impl(thisptr, src);
}