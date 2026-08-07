#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd51f
struct FableCharStringResult_008fd51f
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd51f* FABLE_FASTCALL
FableGetActionName_008fd51f(FableCharStringResult_008fd51f* self, void*)
{ self->build((const char*)19435688, -1); return self; }