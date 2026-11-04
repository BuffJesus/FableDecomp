// NParticleEngine::SetEnvironmentInteraction @ 0x00AEAB20
// (FableWin ?SetEnvironmentInteraction@NParticleEngine@@YIXV?$CCountedPointer@VCAEnvironmentInteraction@NParticleEngine@@@@@Z).
//
// Installs the world's collision/wind hook; the counted pointer arrives by
// value, so the parameter's reference is dropped on return.

#include "fable_particle_engine.h"

void FABLE_FASTCALL NParticleEngine::SetEnvironmentInteraction(
    CCountedPointer<CAEnvironmentInteraction> environmentInteraction)
{
    PEnvironmentInteraction = environmentInteraction;
}