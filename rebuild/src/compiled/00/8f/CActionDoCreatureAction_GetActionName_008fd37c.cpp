#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd37c
struct FableCharStringResult_008fd37c
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd37c* FABLE_FASTCALL
FableGetActionName_008fd37c(FableCharStringResult_008fd37c* self, void*)
{ self->build((const char*)19434920, -1); return self; }