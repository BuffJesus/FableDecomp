#include "rebuild_abi.h"
// OnAnimationLoaded @ 0x008c9dd0  : [this+0x14] = [this+0x10] / 2 (signed) after helper()
struct C { char pad[0x10]; int m_a; int m_b; void OnAnimationLoaded(); };
extern "C" void FABLE_FASTCALL FableBase_8c9dd0(C *self);
void C::OnAnimationLoaded(){
    FableBase_8c9dd0(this);
    m_b = m_a / 2;
}