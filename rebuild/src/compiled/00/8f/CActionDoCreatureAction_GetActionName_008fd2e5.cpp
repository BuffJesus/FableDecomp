#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd2e5
struct FableCharStringResult_008fd2e5
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd2e5* FABLE_FASTCALL
FableGetActionName_008fd2e5(FableCharStringResult_008fd2e5* self, void*)
{ self->build((const char*)19366220, -1); return self; }