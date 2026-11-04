// NParticleEngine::CAEnvironmentInteraction::GetVectorParam @ 0x00415CF0
// (FableWin ?GetVectorParam@CAEnvironmentInteraction@NParticleEngine@@UBE?AVC3DVector@@ABVCCharString@@@Z, slot 5).
// The base interaction knows no named vector parameters: always zero.

#include "fable_particle_engine.h"

C3DVector NParticleEngine::CAEnvironmentInteraction::GetVectorParam(const CCharString&) const
{
    return C3DVector(0.0f, 0.0f, 0.0f);
}