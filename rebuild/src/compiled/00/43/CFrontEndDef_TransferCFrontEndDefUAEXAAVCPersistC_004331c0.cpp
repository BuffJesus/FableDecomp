// CFrontEndDef::Transfer(CPersistContext&) @ 0x004331C0 (FableWin
// ?Transfer@CFrontEndDef@@UAEXAAVCPersistContext@@@Z, vtable 0x01230FBC slot
// 18): the attract-mode movie list, then the three graphic indices, each
// through the binary-only member transfer (vector<CCharString> 0x004331F9,
// long 0x00431102). Retail expands neither template here although both are
// small and their bodies are in the unit: no inline expansion, /O2 registers.
#pragma inline_depth(0)
#include "fable_frontend_def.h"
#include "fable_persist_context.h"

void CFrontEndDef::Transfer(CPersistContext& context)
{
    context.TransferBinaryVector(vAttractModeMovie);
    context.TransferBinaryMember(ErrorMessageBackgroundGraphic);
    context.TransferBinaryMember(ButtonABigGraphic);
    context.TransferBinaryMember(ButtonBBigGraphic);
}
