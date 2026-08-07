#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd77c
struct FableCharStringResult_008fd77c
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd77c* FABLE_FASTCALL
FableGetActionName_008fd77c(FableCharStringResult_008fd77c* self, void*)
{ self->build((const char*)19436696, -1); return self; }