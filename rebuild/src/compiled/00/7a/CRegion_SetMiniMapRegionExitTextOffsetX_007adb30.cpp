#include "rebuild_abi.h"
extern "C" void *FABLE_STDCALL FableHelp_7adb30(void *v);
struct CRegion { void *m_p; };
extern "C" CRegion *FABLE_STDCALL FableSet_7adb30(CRegion *self, void *v){
    self->m_p = FableHelp_7adb30(v); return self; }