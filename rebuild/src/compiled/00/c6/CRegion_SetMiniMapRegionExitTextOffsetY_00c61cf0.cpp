#include "rebuild_abi.h"
// CRegion::SetMiniMapRegionExitTextOffsetY @ 0x00c61cf0
// mov eax,[esp+4]; push eax; call helper; mov ecx,[eax+4]; mov eax,[ecx+0x38]; ret 4
// Resolve a handle via a __cdecl helper, then read a nested field.
struct CInner { char pad00[0x38]; long m_val; };
struct COuter { char pad00[4]; CInner *m_inner; };
extern "C" COuter * FABLE_STDCALL FableResolve_00c61cf0(long handle);
struct CRegion { long SetMiniMapRegionExitTextOffsetY(long handle); };
long CRegion::SetMiniMapRegionExitTextOffsetY(long handle)
{
    COuter *o = FableResolve_00c61cf0(handle);
    return o->m_inner->m_val;
}