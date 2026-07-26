#include "rebuild_abi.h"

// 0x004642f5 CThingObjectDef::Copy
// Retail body is a single 5-byte `jmp rel32` thunk (relocation-masked).
// It tail-calls the real implementation. Modeled as __fastcall(thisptr, src)
// whose only op is a tail-call to an extern impl -> compiler emits `jmp rel32`.

struct CThingObjectDef;

// The real copy implementation this thunk forwards to (address masked).
extern void __fastcall CThingObjectDef_Copy_impl(CThingObjectDef* thisptr, CThingObjectDef* src);

void __fastcall CThingObjectDef_Copy(CThingObjectDef* thisptr, CThingObjectDef* src)
{
    CThingObjectDef_Copy_impl(thisptr, src);
}