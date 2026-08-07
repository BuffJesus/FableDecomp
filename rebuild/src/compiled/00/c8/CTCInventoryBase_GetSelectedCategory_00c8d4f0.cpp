#include "rebuild_abi.h"
// CTCInventoryBase::GetSelectedCategory @ 0x00c8d4f0  __fastcall(this, delta)
struct CTCInventoryBase { char pad[4]; int m_max; int m_cur; };
extern "C" int FABLE_FASTCALL FableGetSel_c8d4f0(CTCInventoryBase *self, int delta){
    int v = self->m_cur + delta;
    if (v > self->m_max) return -1;
    self->m_cur = v; return 0;
}