#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd606
struct FableCharStringResult_008fd606
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd606* FABLE_FASTCALL
FableGetActionName_008fd606(FableCharStringResult_008fd606* self, void*)
{ self->build((const char*)19436008, -1); return self; }