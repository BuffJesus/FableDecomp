#include "rebuild_abi.h"

// CActionDoCreatureAction::GetActionName @ 0x008fdf5b
// One switch-case body of the retail action-name factory. Constructs the hidden
// CCharString result from a relocated string constant with the length sentinel
// -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_008fdf5b
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_008fdf5b* FABLE_FASTCALL
FableGetActionName_008fdf5b(FableCharStringResult_008fdf5b* self, void*)
{
    self->build((const char*)19439480, -1);
    return self;
}