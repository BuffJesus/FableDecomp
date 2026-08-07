#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd6af
struct FableCharStringResult_008fd6af
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd6af* FABLE_FASTCALL
FableGetActionName_008fd6af(FableCharStringResult_008fd6af* self, void*)
{ self->build((const char*)19436328, -1); return self; }