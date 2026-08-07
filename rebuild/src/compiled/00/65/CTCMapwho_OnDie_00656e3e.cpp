#pragma optimize("s",on)
#include "rebuild_abi.h"
// CTCMapwho::OnDie @ 0x00656e3e
struct CTCMapwho;
extern "C" void FABLE_FASTCALL FableH1_656e3e(CTCMapwho *self);
extern "C" void FABLE_CDECL FableH2_656e3e(CTCMapwho *self);
struct CTCMapwho { char dummy; void OnDie(); };
void CTCMapwho::OnDie(){
    if (this) {
        FableH1_656e3e(this);
        FableH2_656e3e(this);
    }
}