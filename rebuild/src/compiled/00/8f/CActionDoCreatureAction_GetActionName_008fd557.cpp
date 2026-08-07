#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd557
struct FableCharStringResult_008fd557
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd557* FABLE_FASTCALL
FableGetActionName_008fd557(FableCharStringResult_008fd557* self, void*)
{ self->build((const char*)19435776, -1); return self; }