#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd2b7
struct FableCharStringResult_008fd2b7
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd2b7* FABLE_FASTCALL
FableGetActionName_008fd2b7(FableCharStringResult_008fd2b7* self, void*)
{ self->build((const char*)19434648, -1); return self; }