#include "rebuild_abi.h"

// CLightningDef::Copy @ 0x00461ed3 is a 5-byte tail-call thunk (jmp rel32).
// It forwards __fastcall(this, src) to the real copy implementation.
// Modeled as __fastcall(thisptr, src) which is byte-identical to __fastcall.

struct CLightningDef;

// The real implementation this thunk tail-calls into (relocation-masked target).
extern "C" void __fastcall CLightningDef_Copy_impl(CLightningDef* thisptr, void* src);

void __fastcall CLightningDef_Copy(CLightningDef* thisptr, void* src)
{
    // Tail call -> compiles to a single `jmp rel32`.
    CLightningDef_Copy_impl(thisptr, src);
}