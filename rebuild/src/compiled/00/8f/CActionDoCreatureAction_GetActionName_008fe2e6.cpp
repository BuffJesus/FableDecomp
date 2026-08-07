#include "rebuild_abi.h"

// CActionDoCreatureAction::GetActionName @ 0x008fe2e6
// One switch-case body of the retail action-name factory. Constructs the hidden
// CCharString result from a relocated string constant with the length sentinel
// -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_008fe2e6
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_008fe2e6* FABLE_FASTCALL
FableGetActionName_008fe2e6(FableCharStringResult_008fe2e6* self, void*)
{
    self->build((const char*)19441056, -1);
    return self;
}