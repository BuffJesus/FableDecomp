#include "rebuild_abi.h"
// CTCHitLocations::GetPCombatFirstHitLocationIntersectionWithFiniteLine @ 0x00816aa0
extern "C" void FABLE_FASTCALL FableImpl_816aa0(void *self);
extern "C" void FABLE_FASTCALL FableThunk_816aa0(void *self){ FableImpl_816aa0(self); }