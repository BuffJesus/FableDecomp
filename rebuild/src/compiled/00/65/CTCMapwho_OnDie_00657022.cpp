#pragma optimize("s",on)
#include "rebuild_abi.h"
// CTCMapwho::OnDie @ 0x00657022
struct CTCMapwho;
extern "C" void FABLE_FASTCALL FableH1_657022(CTCMapwho *self);
extern "C" void FABLE_CDECL FableH2_657022(CTCMapwho *self);
struct CTCMapwho { char dummy; void OnDie(); };
void CTCMapwho::OnDie(){
    if (this) {
        FableH1_657022(this);
        FableH2_657022(this);
    }
}