#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd595
struct FableCharStringResult_008fd595
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd595* FABLE_FASTCALL
FableGetActionName_008fd595(FableCharStringResult_008fd595* self, void*)
{ self->build((const char*)19435824, -1); return self; }