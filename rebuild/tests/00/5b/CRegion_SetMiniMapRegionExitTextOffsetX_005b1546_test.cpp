#include "rebuild_abi.h"
#include <cstdio>
extern "C" void *FableRegionXform_5b1546(void *cur){ return (void*)((char*)cur + 1); }
struct CRegion { void *f0; CRegion *SetMiniMapRegionExitTextOffsetX(); };
CRegion *CRegion::SetMiniMapRegionExitTextOffsetX(){ f0=FableRegionXform_5b1546(f0); return this; }
int main(){
    CRegion r; r.f0=(void*)0x10;
    CRegion *g = r.SetMiniMapRegionExitTextOffsetX();
    if(g!=&r||r.f0!=(void*)0x11){std::printf("BAD\n");return 1;}
    std::printf("REGIONX_OK\n"); return 0;
}