#include "rebuild_abi.h"
// CEngineInternalPrimitiveMeshBase::GetBoundingSphereRadiusWorldSpace @ 0x00ba8010
// eax=[ecx+0x50]; fld [eax+0x38]; al=1; fmul [ecx+0x58]; ecx=[esp+4]; fstp [ecx]; ret 4
struct SubObj { char pad[0x38]; float m_r; };
struct CEngineInternalPrimitiveMeshBase { char pad[0x50]; SubObj *m_sub; char gap[4]; float m_scale; bool GetR(float *out); };
bool CEngineInternalPrimitiveMeshBase::GetR(float *out)
{
    *out = m_sub->m_r * m_scale;
    return true;
}