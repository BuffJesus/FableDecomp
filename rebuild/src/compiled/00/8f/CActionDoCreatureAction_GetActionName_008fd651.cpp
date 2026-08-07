#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd651
struct FableCharStringResult_008fd651
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd651* FABLE_FASTCALL
FableGetActionName_008fd651(FableCharStringResult_008fd651* self, void*)
{ self->build((const char*)19436168, -1); return self; }