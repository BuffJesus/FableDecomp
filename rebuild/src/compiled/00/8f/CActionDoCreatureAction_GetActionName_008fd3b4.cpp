#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd3b4
struct FableCharStringResult_008fd3b4
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd3b4* FABLE_FASTCALL
FableGetActionName_008fd3b4(FableCharStringResult_008fd3b4* self, void*)
{ self->build((const char*)19435036, -1); return self; }