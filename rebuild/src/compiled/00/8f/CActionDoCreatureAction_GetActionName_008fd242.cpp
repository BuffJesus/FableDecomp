#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd242
struct FableCharStringResult_008fd242
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd242* FABLE_FASTCALL
FableGetActionName_008fd242(FableCharStringResult_008fd242* self, void*)
{ self->build((const char*)19434568, -1); return self; }