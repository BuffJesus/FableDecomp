// NParticleEngine::Initialise @ 0x00AEBE20 (FableWin ?Initialise@NParticleEngine@@YIXXZ).
//
// Creates the engine's three singletons: the emitter database, the emitter
// manager and the particle-system-component manager.

#include "fable_particle_engine.h"

void FABLE_FASTCALL NParticleEngine::Initialise()
{
    PEmitterDatabase.Reset(new CParticleEmitterDatabase);
    PEmitterManager.Reset(new CParticleEmitterManager);
    PPSCManager.Reset(new CPSCManager);
}