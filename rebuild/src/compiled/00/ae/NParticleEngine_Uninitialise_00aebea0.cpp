// NParticleEngine::Uninitialise @ 0x00AEBEA0 (FableWin ?Uninitialise@NParticleEngine@@YIXXZ).
//
// Drops the engine's references to its singletons: the emitter manager
// first, then the emitter database and the component manager.

#include "fable_particle_engine.h"

void FABLE_FASTCALL NParticleEngine::Uninitialise()
{
    PEmitterManager.ReleaseData();
    PEmitterDatabase.ReleaseData();
    PPSCManager.ReleaseData();
}