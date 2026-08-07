#include "rebuild_abi.h"

// CActionDoCreatureAction::GetActionName @ 0x008fdf22
// One switch-case body of the retail action-name factory. Constructs the hidden
// CCharString result from a relocated string constant with the length sentinel
// -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_008fdf22
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_008fdf22* FABLE_FASTCALL
FableGetActionName_008fdf22(FableCharStringResult_008fdf22* self, void*)
{
    self->build((const char*)19439400, -1);
    return self;
}