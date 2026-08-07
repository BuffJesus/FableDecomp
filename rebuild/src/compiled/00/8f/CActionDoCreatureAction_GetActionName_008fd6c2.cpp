#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd6c2
struct FableCharStringResult_008fd6c2
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd6c2* FABLE_FASTCALL
FableGetActionName_008fd6c2(FableCharStringResult_008fd6c2* self, void*)
{ self->build((const char*)19436352, -1); return self; }