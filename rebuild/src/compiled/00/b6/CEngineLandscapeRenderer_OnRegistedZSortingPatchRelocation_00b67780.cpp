#include "rebuild_abi.h"
// CEngineLandscapeRenderer::OnRegistedZSortingPatchRelocation @ 0x00b67780
// eax=[ecx+0x5e4]; [eax+idx*8]=val; ret 8
struct Slot8 { void *a; void *b; };
struct CEngineLandscapeRenderer { char pad[0x5e4]; Slot8 *m_arr; void OnReg(unsigned idx, void *val); };
void CEngineLandscapeRenderer::OnReg(unsigned idx, void *val)
{
    ((void**)&m_arr[idx])[0] = val;
}