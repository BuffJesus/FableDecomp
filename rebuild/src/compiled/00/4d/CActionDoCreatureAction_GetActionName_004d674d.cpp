#include "rebuild_abi.h"

// CActionDoCreatureAction::GetActionName @ 0x004d674d
struct FableCharStringResult_004d674d
{
    void* build(const char* text, int length);
};

extern "C" FableCharStringResult_004d674d* FABLE_FASTCALL
FableGetActionName_004d674d(FableCharStringResult_004d674d* self, void*)
{
    self->build((const char*)19130188, -1);
    return self;
}