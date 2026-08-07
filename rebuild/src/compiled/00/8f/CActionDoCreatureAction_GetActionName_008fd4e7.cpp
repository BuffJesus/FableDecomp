#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd4e7
struct FableCharStringResult_008fd4e7
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd4e7* FABLE_FASTCALL
FableGetActionName_008fd4e7(FableCharStringResult_008fd4e7* self, void*)
{ self->build((const char*)19435572, -1); return self; }