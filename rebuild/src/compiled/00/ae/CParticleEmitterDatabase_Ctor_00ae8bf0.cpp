// NParticleEngine::CParticleEmitterDatabase::CParticleEmitterDatabase (retail 0x00AE8BF0, Ego_r particle_emitter_database):
// an empty database (symbol map, no emitters, no bank, no file names) whose first emitter handle will be 1.
#include "fable_particle_engine.h"

namespace NParticleEngine
{
    CParticleEmitterDatabase::CParticleEmitterDatabase()
    {
        CurrentHandle = 1;
    }
}
