#include "rebuild_abi.h"

// CActionDoCreatureAction::GetActionName @ 0x008fd9ff
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_008fd9ff
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_008fd9ff* FABLE_FASTCALL
FableGetActionName_008fd9ff(FableCharStringResult_008fd9ff* self, void*)
{
    self->build((const char*)19437688, -1);
    return self;
}