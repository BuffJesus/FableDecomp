#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd5bb
struct FableCharStringResult_008fd5bb
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd5bb* FABLE_FASTCALL
FableGetActionName_008fd5bb(FableCharStringResult_008fd5bb* self, void*)
{ self->build((const char*)19435872, -1); return self; }