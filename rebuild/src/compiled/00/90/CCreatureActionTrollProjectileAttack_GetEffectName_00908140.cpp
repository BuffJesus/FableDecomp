#include "rebuild_abi.h"

// CCreatureAction_TrollProjectileAttack::GetEffectName @ 0x00908140
// GetEffectName: result buffer arg at [esp+4]; constructs the hidden CCharString
// from a relocated string constant, returns true (al=1), callee-cleans (ret 4).
// Shape: mov ecx,[esp+4]; push STR; call CCharString_ctor; mov al,1; ret 4.
struct FableEffectResult_00908140 { void* ctor(const char* text); };

extern "C" char FABLE_STDCALL FableGetEffectName_00908140(FableEffectResult_00908140* self)
{
    self->ctor((const char*)19456924);
    return 1;
}