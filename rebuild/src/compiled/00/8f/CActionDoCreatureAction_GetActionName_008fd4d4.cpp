#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd4d4
struct FableCharStringResult_008fd4d4
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd4d4* FABLE_FASTCALL
FableGetActionName_008fd4d4(FableCharStringResult_008fd4d4* self, void*)
{ self->build((const char*)19435532, -1); return self; }