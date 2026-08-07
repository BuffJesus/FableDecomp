#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd446
struct FableCharStringResult_008fd446
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd446* FABLE_FASTCALL
FableGetActionName_008fd446(FableCharStringResult_008fd446* self, void*)
{ self->build((const char*)19435300, -1); return self; }