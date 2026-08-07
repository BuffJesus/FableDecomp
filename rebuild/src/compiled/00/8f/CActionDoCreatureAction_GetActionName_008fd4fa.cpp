#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd4fa
struct FableCharStringResult_008fd4fa
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd4fa* FABLE_FASTCALL
FableGetActionName_008fd4fa(FableCharStringResult_008fd4fa* self, void*)
{ self->build((const char*)19435600, -1); return self; }