#include "engine/CEnginePrimitiveManagerParticleDecalGroup.h"  // retyped onto the PDB layout; byte parity re-verified
#include "rebuild_abi.h"
// CEnginePrimitiveManagerParticleDecalGroup_Methods::RenderPrimitive @ 0x00bb6de0
// xor eax,eax; ret 0xc  -> returns ERendered(0); 3 stack args (thiscall)
struct CEngineInternalPrimitiveBase; struct CEngineSubPrimitiveChain;
struct CEnginePrimitiveManagerParticleDecalGroup_Methods : CEnginePrimitiveManagerParticleDecalGroup {
    int RenderPrimitive(CEngineInternalPrimitiveBase *p, int layer, CEngineSubPrimitiveChain *c);
};
int CEnginePrimitiveManagerParticleDecalGroup_Methods::RenderPrimitive(CEngineInternalPrimitiveBase *p, int layer, CEngineSubPrimitiveChain *c)
{
    return 0;
}