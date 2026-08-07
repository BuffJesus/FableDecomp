#include "rebuild_abi.h"
#include <cstdio>
struct CInner { char pad00[0x38]; long m_val; };
struct COuter { char pad00[4]; CInner *m_inner; };
static CInner g_in_00c61cf0; static COuter g_out_00c61cf0;
extern "C" COuter * FABLE_STDCALL FableResolve_00c61cf0(long handle){ g_in_00c61cf0.m_val=handle+1; g_out_00c61cf0.m_inner=&g_in_00c61cf0; return &g_out_00c61cf0; }
struct CRegion { long SetMiniMapRegionExitTextOffsetY(long handle); };
long CRegion::SetMiniMapRegionExitTextOffsetY(long handle){ COuter *o=FableResolve_00c61cf0(handle); return o->m_inner->m_val; }
int main(){
    CRegion r;
    if(r.SetMiniMapRegionExitTextOffsetY(41)!=42){std::printf("BAD\n");return 1;}
    std::printf("REGION1CF0_OK\n"); return 0;
}