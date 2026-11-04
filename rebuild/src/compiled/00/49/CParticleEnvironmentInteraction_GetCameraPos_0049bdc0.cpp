// CParticleEnvironmentInteraction::GetCameraPos @ 0x0049BDC0
// (FableWin ?GetCameraPos@CParticleEnvironmentInteraction@@UBEXAAVC3DVector@@@Z, slot 6).
//
// With a world, the particle camera is the world's game LOD centre;
// otherwise the fixed CameraPos.

#include "fable_particle_engine.h"
#include "fable_world.h"

void CParticleEnvironmentInteraction::GetCameraPos(C3DVector& position) const
{
    if (PWorld != 0)
        position = PWorld->GetGameLODCentre();
    else
        position = CameraPos;
}