#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd689
struct FableCharStringResult_008fd689
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd689* FABLE_FASTCALL
FableGetActionName_008fd689(FableCharStringResult_008fd689* self, void*)
{ self->build((const char*)19366204, -1); return self; }