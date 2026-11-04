#include "fable_particle_engine.h"

class CMeshDataBank;

namespace NParticleEngine
{
    const CGraphicDataBank* PGraphicDataBank = 0;
    const CMeshDataBank* PMeshDataBank = 0;

    void FABLE_FASTCALL SetGraphicBank(const CGraphicDataBank& bank)
    {
        PGraphicDataBank = &bank;
    }

    void FABLE_FASTCALL SetMeshBank(const CMeshDataBank& bank)
    {
        PMeshDataBank = &bank;
    }
}
