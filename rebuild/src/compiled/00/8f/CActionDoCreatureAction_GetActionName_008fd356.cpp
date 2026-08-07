#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd356
struct FableCharStringResult_008fd356
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd356* FABLE_FASTCALL
FableGetActionName_008fd356(FableCharStringResult_008fd356* self, void*)
{ self->build((const char*)19434852, -1); return self; }