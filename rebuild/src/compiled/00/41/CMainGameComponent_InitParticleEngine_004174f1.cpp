// CMainGameComponent::InitParticleEngine @ 0x004174F1 (233 bytes).
// FableWin ?InitParticleEngine@CMainGameComponent@@IAEXXZ. Starts the
// particle engine, hands it a world-backed environment interaction and opens
// the main particle bank.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_particle_engine.h"
#include "fable_world.h"

void CMainGameComponent::InitParticleEngine()
{
    NParticleEngine::Initialise();

    CCountedPointer<NParticleEngine::CAEnvironmentInteraction> environmentInteraction(
        new CParticleEnvironmentInteraction(*PWorld));
    NParticleEngine::SetEnvironmentInteraction(environmentInteraction);

    CCharString bankName("PARTICLE_MAIN");
    NParticleEngine::GetParticleEmitterDatabase().OpenBank(bankName);
}