#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd5a8
struct FableCharStringResult_008fd5a8
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd5a8* FABLE_FASTCALL
FableGetActionName_008fd5a8(FableCharStringResult_008fd5a8* self, void*)
{ self->build((const char*)19435848, -1); return self; }