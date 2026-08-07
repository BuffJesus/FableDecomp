#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd343
struct FableCharStringResult_008fd343
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd343* FABLE_FASTCALL
FableGetActionName_008fd343(FableCharStringResult_008fd343* self, void*)
{ self->build((const char*)19434824, -1); return self; }