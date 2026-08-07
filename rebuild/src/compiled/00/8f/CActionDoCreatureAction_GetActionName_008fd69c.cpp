#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd69c
struct FableCharStringResult_008fd69c
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd69c* FABLE_FASTCALL
FableGetActionName_008fd69c(FableCharStringResult_008fd69c* self, void*)
{ self->build((const char*)19436304, -1); return self; }