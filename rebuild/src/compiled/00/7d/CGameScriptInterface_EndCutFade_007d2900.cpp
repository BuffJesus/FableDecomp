#include "rebuild_abi.h"
// CGameScriptInterface::EndCutFade @ 0x007d2900
// push esi; mov esi,ecx; call h1; push 7; mov ecx,esi; call h2; pop esi; ret
struct FableEndCut_007d2900
{
    void FABLE_FASTCALL h1();
    void h2(int v);
    void EndCutFade();
};
void FableEndCut_007d2900::EndCutFade()
{
    this->h1();
    this->h2(7);
}