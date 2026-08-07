#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd6fa
struct FableCharStringResult_008fd6fa
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd6fa* FABLE_FASTCALL
FableGetActionName_008fd6fa(FableCharStringResult_008fd6fa* self, void*)
{ self->build((const char*)19436452, -1); return self; }