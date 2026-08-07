#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd6d5
struct FableCharStringResult_008fd6d5
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd6d5* FABLE_FASTCALL
FableGetActionName_008fd6d5(FableCharStringResult_008fd6d5* self, void*)
{ self->build((const char*)19436380, -1); return self; }