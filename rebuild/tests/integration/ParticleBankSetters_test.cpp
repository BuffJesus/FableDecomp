#include "fable_particle_engine.h"
#include <stdio.h>

class CMeshDataBank;
namespace NParticleEngine
{
    extern const CGraphicDataBank* PGraphicDataBank;
    extern const CMeshDataBank* PMeshDataBank;
    void FABLE_FASTCALL SetMeshBank(const CMeshDataBank& bank);
}

static char g_graphics[3][16], g_meshes[3][16];

int main()
{
    using namespace NParticleEngine;
    if (PGraphicDataBank || PMeshDataBank) return 1;
    int cases = 0;
    for (int graphic = 0; graphic < 3; ++graphic)
    for (int mesh = 0; mesh < 3; ++mesh)
    {
        const CGraphicDataBank* g = (const CGraphicDataBank*)g_graphics[graphic];
        const CMeshDataBank* m = (const CMeshDataBank*)g_meshes[mesh];
        SetGraphicBank(*g);
        if (PGraphicDataBank != g) return 2;
        SetMeshBank(*m);
        if (PGraphicDataBank != g || PMeshDataBank != m) return 3;
        ++cases;
    }
    printf("PARTICLE_BANK_SETTERS_BEHAVIOR_PASS cases=%d\n", cases);
    return 0;
}
