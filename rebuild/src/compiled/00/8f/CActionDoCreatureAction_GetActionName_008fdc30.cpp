#include "rebuild_abi.h"

// CActionDoCreatureAction::GetActionName @ 0x008fdc30
// One switch-case body of the retail action-name factory. Constructs the hidden
// CCharString result from a relocated string constant with the length sentinel
// -1, then returns the result pointer (this) in esi/eax.
// Shape: push esi; push -1; push STR; mov esi,ecx; call ctor; mov eax,esi; ret.
struct FableCharStringResult_008fdc30
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_008fdc30* FABLE_FASTCALL
FableGetActionName_008fdc30(FableCharStringResult_008fdc30* self, void*)
{
    self->build((const char*)19438456, -1);
    return self;
}