// NParticleEngine::CAEnvironmentInteraction::GetGravityVector @ 0x00415CD0
// (FableWin ?GetGravityVector@CAEnvironmentInteraction@NParticleEngine@@UBE?AVC3DVector@@XZ, slot 4).
// Default gravity: straight down at 9.82 units/s^2.

#include "fable_particle_engine.h"

C3DVector NParticleEngine::CAEnvironmentInteraction::GetGravityVector() const
{
    return C3DVector(0.0f, 0.0f, -9.82f);
}