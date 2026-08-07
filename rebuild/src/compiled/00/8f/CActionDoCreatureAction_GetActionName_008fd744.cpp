#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd744
struct FableCharStringResult_008fd744
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd744* FABLE_FASTCALL
FableGetActionName_008fd744(FableCharStringResult_008fd744* self, void*)
{ self->build((const char*)19436604, -1); return self; }