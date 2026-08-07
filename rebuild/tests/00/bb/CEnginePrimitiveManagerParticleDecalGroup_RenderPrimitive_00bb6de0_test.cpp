#include "rebuild_abi.h"
#include <cstdio>
struct CEngineInternalPrimitiveBase; struct CEngineSubPrimitiveChain;
struct CEnginePrimitiveManagerParticleDecalGroup {
    int RenderPrimitive(CEngineInternalPrimitiveBase *p, int layer, CEngineSubPrimitiveChain *c);
};
int CEnginePrimitiveManagerParticleDecalGroup::RenderPrimitive(CEngineInternalPrimitiveBase *p, int layer, CEngineSubPrimitiveChain *c){ return 0; }
int main(){
    CEnginePrimitiveManagerParticleDecalGroup g;
    if(g.RenderPrimitive(0,0,0)!=0){std::printf("BAD\n");return 1;}
    std::printf("RENDPRIM_OK\n"); return 0;
}