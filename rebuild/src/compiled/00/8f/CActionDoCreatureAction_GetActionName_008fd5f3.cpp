#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd5f3
struct FableCharStringResult_008fd5f3
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd5f3* FABLE_FASTCALL
FableGetActionName_008fd5f3(FableCharStringResult_008fd5f3* self, void*)
{ self->build((const char*)19435984, -1); return self; }