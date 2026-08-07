#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd71f
struct FableCharStringResult_008fd71f
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd71f* FABLE_FASTCALL
FableGetActionName_008fd71f(FableCharStringResult_008fd71f* self, void*)
{ self->build((const char*)19436524, -1); return self; }