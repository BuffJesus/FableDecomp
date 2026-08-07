#include "rebuild_abi.h"

// CActionDoCreatureAction::GetActionName @ 0x008fe50e
// One switch-case body of the retail action-name factory. Constructs the hidden
// CCharString result from a relocated string constant with the length sentinel
// -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_008fe50e
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_008fe50e* FABLE_FASTCALL
FableGetActionName_008fe50e(FableCharStringResult_008fe50e* self, void*)
{
    self->build((const char*)19441876, -1);
    return self;
}