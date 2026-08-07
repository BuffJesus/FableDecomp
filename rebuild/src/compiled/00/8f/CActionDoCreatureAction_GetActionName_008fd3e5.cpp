#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd3e5
struct FableCharStringResult_008fd3e5
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd3e5* FABLE_FASTCALL
FableGetActionName_008fd3e5(FableCharStringResult_008fd3e5* self, void*)
{ self->build((const char*)19435128, -1); return self; }