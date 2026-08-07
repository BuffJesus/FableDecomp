#include "rebuild_abi.h"
// CRegion::SetMiniMapRegionExitTextOffsetX @ 0x004baa30
// eax=[esp+8]; push eax; call helper; ecx=[esp+4]; [ecx]=eax; eax=ecx; ret 8
extern "C" void *FABLE_STDCALL FableHelp_baa30(void *v);
struct CRegion { void *m_p; };
extern "C" CRegion *FABLE_STDCALL FableSet_baa30(CRegion *self, void *v);
extern "C" CRegion *FABLE_STDCALL FableSet_baa30(CRegion *self, void *v){
    self->m_p = FableHelp_baa30(v);
    return self;
}