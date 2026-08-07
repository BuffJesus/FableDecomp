#pragma optimize("s",on)
#include "rebuild_abi.h"
// CRegion::SetMiniMapRegionExitTextOffsetX @ 0x005b1546
// push esi; mov esi,ecx; push [esi]; call helper; mov [esi],eax; pop ecx; mov eax,esi; pop esi; ret
// Replace this->f0 via a __cdecl transform helper, then return this.
struct CRegion;
extern "C" void *FableRegionXform_5b1546(void *cur);
struct CRegionSub { void *f0; };
struct CRegion { void *f0; CRegion *SetMiniMapRegionExitTextOffsetX(); };
CRegion *CRegion::SetMiniMapRegionExitTextOffsetX()
{
    f0 = FableRegionXform_5b1546(f0);
    return this;
}