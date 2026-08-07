#include "rebuild_abi.h"

// CActionDoCreatureAction::GetActionName @ 0x004d532e
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d532e
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d532e* FABLE_FASTCALL
FableGetActionName_004d532e(FableCharStringResult_004d532e* self, void*)
{
    self->build((const char*)19123120, -1);
    return self;
}