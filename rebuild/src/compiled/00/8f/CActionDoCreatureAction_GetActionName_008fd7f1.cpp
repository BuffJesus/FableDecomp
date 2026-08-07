#include "rebuild_abi.h"

// CActionDoCreatureAction::GetActionName @ 0x008fd7f1
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_008fd7f1
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_008fd7f1* FABLE_FASTCALL
FableGetActionName_008fd7f1(FableCharStringResult_008fd7f1* self, void*)
{
    self->build((const char*)19436896, -1);
    return self;
}