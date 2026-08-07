#include "rebuild_abi.h"
// CEnginePrimitiveManagerParticleDecalGroup::RenderPrimitive @ 0x00bb6de0
// xor eax,eax; ret 0xc  -> returns ERendered(0); 3 stack args (thiscall)
struct CEngineInternalPrimitiveBase; struct CEngineSubPrimitiveChain;
struct CEnginePrimitiveManagerParticleDecalGroup {
    int RenderPrimitive(CEngineInternalPrimitiveBase *p, int layer, CEngineSubPrimitiveChain *c);
};
int CEnginePrimitiveManagerParticleDecalGroup::RenderPrimitive(CEngineInternalPrimitiveBase *p, int layer, CEngineSubPrimitiveChain *c)
{
    return 0;
}