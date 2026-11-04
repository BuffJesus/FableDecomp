// NParticleEngine::CAEnvironmentInteraction::IsWindActive @ 0x00415CC0
// (FableWin ?IsWindActive@CAEnvironmentInteraction@NParticleEngine@@UBE_NXZ, slot 2).
// The base interaction always reports wind.

#include "fable_particle_engine.h"

bool NParticleEngine::CAEnvironmentInteraction::IsWindActive() const
{
    return true;
}
