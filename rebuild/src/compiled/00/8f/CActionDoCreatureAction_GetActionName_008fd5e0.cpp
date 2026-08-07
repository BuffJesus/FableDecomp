#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd5e0
struct FableCharStringResult_008fd5e0
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd5e0* FABLE_FASTCALL
FableGetActionName_008fd5e0(FableCharStringResult_008fd5e0* self, void*)
{ self->build((const char*)19435960, -1); return self; }