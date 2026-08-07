#include "rebuild_abi.h"

// CActionDoCreatureAction::GetActionName @ 0x004d2d5f
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d2d5f
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d2d5f* FABLE_FASTCALL
FableGetActionName_004d2d5f(FableCharStringResult_004d2d5f* self, void*)
{
    self->build((const char*)19112684, -1);
    return self;
}