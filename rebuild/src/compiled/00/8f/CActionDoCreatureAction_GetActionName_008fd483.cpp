#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd483
struct FableCharStringResult_008fd483
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd483* FABLE_FASTCALL
FableGetActionName_008fd483(FableCharStringResult_008fd483* self, void*)
{ self->build((const char*)19435412, -1); return self; }