#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd3a1
struct FableCharStringResult_008fd3a1
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd3a1* FABLE_FASTCALL
FableGetActionName_008fd3a1(FableCharStringResult_008fd3a1* self, void*)
{ self->build((const char*)19435016, -1); return self; }