#include "rebuild_abi.h"

// 0x0045d500 CCameraManagerDef::Copy -- 5-byte tail-call thunk (single jmp rel32, reloc-masked).
// Modeled as a free __fastcall(this, arg) that tail-calls the real implementation.

struct CCameraManagerDef;

// The real copy implementation this thunk forwards to (address relocation-masked).
extern void __fastcall CCameraManagerDef_Copy_impl(CCameraManagerDef* thisptr, void* other);

void __fastcall CCameraManagerDef_Copy(CCameraManagerDef* thisptr, void* other)
{
    CCameraManagerDef_Copy_impl(thisptr, other);
}