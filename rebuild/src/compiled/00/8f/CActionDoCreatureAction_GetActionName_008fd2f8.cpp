#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd2f8
struct FableCharStringResult_008fd2f8
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd2f8* FABLE_FASTCALL
FableGetActionName_008fd2f8(FableCharStringResult_008fd2f8* self, void*)
{ self->build((const char*)19434668, -1); return self; }