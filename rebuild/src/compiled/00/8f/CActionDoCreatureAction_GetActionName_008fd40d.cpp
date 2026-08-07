#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd40d
struct FableCharStringResult_008fd40d
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd40d* FABLE_FASTCALL
FableGetActionName_008fd40d(FableCharStringResult_008fd40d* self, void*)
{ self->build((const char*)19435224, -1); return self; }