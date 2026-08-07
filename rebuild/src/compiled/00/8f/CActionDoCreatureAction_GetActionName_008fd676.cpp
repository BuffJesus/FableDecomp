#include "rebuild_abi.h"
// CActionDoCreatureAction::GetActionName @ 0x008fd676
struct FableCharStringResult_008fd676
{ void* build(const char* text, int length); };
extern "C" FableCharStringResult_008fd676* FABLE_FASTCALL
FableGetActionName_008fd676(FableCharStringResult_008fd676* self, void*)
{ self->build((const char*)19436272, -1); return self; }