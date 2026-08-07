#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd63e
struct FableCharStringResult_008fd63e
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd63e* FABLE_FASTCALL
FableGetActionName_008fd63e(FableCharStringResult_008fd63e* self, void*)
{ self->build((const char*)19436128, -1); return self; }