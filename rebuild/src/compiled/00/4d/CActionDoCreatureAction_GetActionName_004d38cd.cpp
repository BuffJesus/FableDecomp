#include "rebuild_abi.h"

// CActionDoCreatureAction::GetActionName @ 0x004d38cd
// One switch-case body of the retail action-name factory. Each case constructs
// the hidden CCharString result from a relocated string constant with the
// length sentinel -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_004d38cd
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d38cd* FABLE_FASTCALL
FableGetActionName_004d38cd(FableCharStringResult_004d38cd* self, void*)
{
    self->build((const char*)19115488, -1);
    return self;
}