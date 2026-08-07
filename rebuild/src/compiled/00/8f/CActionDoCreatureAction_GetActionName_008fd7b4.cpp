#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd7b4
struct FableCharStringResult_008fd7b4
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd7b4* FABLE_FASTCALL
FableGetActionName_008fd7b4(FableCharStringResult_008fd7b4* self, void*)
{ self->build((const char*)19436800, -1); return self; }