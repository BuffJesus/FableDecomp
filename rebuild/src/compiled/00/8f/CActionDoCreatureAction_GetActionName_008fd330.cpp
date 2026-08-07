#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd330
struct FableCharStringResult_008fd330
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd330* FABLE_FASTCALL
FableGetActionName_008fd330(FableCharStringResult_008fd330* self, void*)
{ self->build((const char*)19434784, -1); return self; }