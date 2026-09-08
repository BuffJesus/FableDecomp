#include "engine/CCreatureAction_TrollProjectileAttack.h"
#include "rebuild_abi.h"

struct FableEffectResult_00908140 { void* ctor(const char* text); };
extern "C" char FABLE_STDCALL FableGetEffectName_00908140(FableEffectResult_00908140* result)
{
    result->ctor((const char*)19456924);
    return 1;
}
