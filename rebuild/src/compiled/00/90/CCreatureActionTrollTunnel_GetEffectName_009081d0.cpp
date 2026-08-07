#include "rebuild_abi.h"

// CCreatureAction_TrollTunnel::GetEffectName @ 0x009081d0
// GetEffectName: result buffer arg at [esp+4]; constructs the hidden CCharString
// from a relocated string constant, returns true (al=1), callee-cleans (ret 4).
// Shape: mov ecx,[esp+4]; push STR; call CCharString_ctor; mov al,1; ret 4.
struct FableEffectResult_009081d0 { void* ctor(const char* text); };

extern "C" char FABLE_STDCALL FableGetEffectName_009081d0(FableEffectResult_009081d0* self)
{
    self->ctor((const char*)19456924);
    return 1;
}