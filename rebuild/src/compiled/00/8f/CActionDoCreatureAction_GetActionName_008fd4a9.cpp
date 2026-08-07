#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd4a9
struct FableCharStringResult_008fd4a9
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd4a9* FABLE_FASTCALL
FableGetActionName_008fd4a9(FableCharStringResult_008fd4a9* self, void*)
{ self->build((const char*)19435480, -1); return self; }