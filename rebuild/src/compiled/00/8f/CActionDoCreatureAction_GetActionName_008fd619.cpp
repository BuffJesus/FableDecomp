#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd619
struct FableCharStringResult_008fd619
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd619* FABLE_FASTCALL
FableGetActionName_008fd619(FableCharStringResult_008fd619* self, void*)
{ self->build((const char*)19436032, -1); return self; }