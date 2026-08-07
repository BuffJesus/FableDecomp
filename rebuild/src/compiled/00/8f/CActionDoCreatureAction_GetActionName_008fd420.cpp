#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd420
struct FableCharStringResult_008fd420
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd420* FABLE_FASTCALL
FableGetActionName_008fd420(FableCharStringResult_008fd420* self, void*)
{ self->build((const char*)19435256, -1); return self; }