#include "rebuild_abi.h"
// CRegion::SetMiniMapRegionExitTextOffsetX @ 0x00515700
extern "C" void *FABLE_STDCALL FableHelp_515700(void *v);
struct CRegion { void *m_p; };
extern "C" CRegion *FABLE_STDCALL FableSet_515700(CRegion *self, void *v);
extern "C" CRegion *FABLE_STDCALL FableSet_515700(CRegion *self, void *v){
    self->m_p = FableHelp_515700(v);
    return self;
}