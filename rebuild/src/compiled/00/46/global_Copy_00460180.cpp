#include "rebuild_abi.h"

// 0x00460180 CSpecialAbilitiesGhostSwordDef::Copy
// Retail is a 5-byte tail-call thunk: e9 rel32  ->  jmp <target>
// (rel32 is relocation-masked in parity). Model as a __fastcall member
// thunk that tail-calls the real copy implementation with the same args.

struct CSpecialAbilitiesGhostSwordDef;

// The real target (relocation-masked). Same signature so it tail-calls.
extern void __fastcall CSpecialAbilitiesGhostSwordDef_Copy_impl(
    CSpecialAbilitiesGhostSwordDef* thisptr, void* edx, void* src);

void __fastcall CSpecialAbilitiesGhostSwordDef_Copy(
    CSpecialAbilitiesGhostSwordDef* thisptr, void* edx, void* src)
{
    // Tail call -> compiles to a single `jmp rel32`.
    CSpecialAbilitiesGhostSwordDef_Copy_impl(thisptr, edx, src);
}