#include "rebuild_abi.h"

// CActionDoCreatureAction::GetActionName @ 0x004d6026
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d6026
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d6026* FABLE_FASTCALL
FableGetActionName_004d6026(FableCharStringResult_004d6026* self, void*)
{
    self->build((const char*)19127608, -1);
    return self;
}