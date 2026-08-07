#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd78f
struct FableCharStringResult_008fd78f
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd78f* FABLE_FASTCALL
FableGetActionName_008fd78f(FableCharStringResult_008fd78f* self, void*)
{ self->build((const char*)19436720, -1); return self; }