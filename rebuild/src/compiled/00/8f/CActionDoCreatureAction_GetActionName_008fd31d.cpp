#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd31d
struct FableCharStringResult_008fd31d
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd31d* FABLE_FASTCALL
FableGetActionName_008fd31d(FableCharStringResult_008fd31d* self, void*)
{ self->build((const char*)19434752, -1); return self; }