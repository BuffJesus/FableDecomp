#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd496
struct FableCharStringResult_008fd496
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd496* FABLE_FASTCALL
FableGetActionName_008fd496(FableCharStringResult_008fd496* self, void*)
{ self->build((const char*)19435456, -1); return self; }