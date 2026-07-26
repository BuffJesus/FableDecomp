#include "rebuild_abi.h"

// 0x00460584 CSpecialAbilitiesDrunkennessDef::Copy
// Retail body is a single `jmp rel32` tail-call thunk (5 bytes: e9 00000000).
// The rel32 target is relocation-masked, so any extern callee produces byte-identical
// output once the __fastcall->__fastcall lowering is applied by the lander.

struct CSpecialAbilitiesDrunkennessDef;

// The forwarded-to copy implementation (real target relocation-masked).
extern void __fastcall CSpecialAbilitiesDrunkennessDef_CopyImpl(
    CSpecialAbilitiesDrunkennessDef* thisptr, void* other);

// Tail-call thunk: forwards both args unchanged and returns via jmp.
void __fastcall CSpecialAbilitiesDrunkennessDef_Copy(
    CSpecialAbilitiesDrunkennessDef* thisptr, void* other)
{
    CSpecialAbilitiesDrunkennessDef_CopyImpl(thisptr, other);
}