#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd433
struct FableCharStringResult_008fd433
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd433* FABLE_FASTCALL
FableGetActionName_008fd433(FableCharStringResult_008fd433* self, void*)
{ self->build((const char*)19435276, -1); return self; }