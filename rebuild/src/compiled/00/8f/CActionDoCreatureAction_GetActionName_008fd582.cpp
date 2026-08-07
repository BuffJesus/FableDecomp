#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd582
struct FableCharStringResult_008fd582
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd582* FABLE_FASTCALL
FableGetActionName_008fd582(FableCharStringResult_008fd582* self, void*)
{ self->build((const char*)19435800, -1); return self; }