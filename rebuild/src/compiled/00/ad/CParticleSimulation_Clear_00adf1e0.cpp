// CParticleSimulation::Clear (retail 0x00ADF1E0, Ego_r 0x00BEAE90): reset the simulation to an empty state at the default
// 1/15 s timestep and hand any particle arrays back to their movable-resource pool.
#include "fable_particle_simulation.h"

void CParticleSimulation::Clear()
{
    Timestep = 1.0f / 15.0f;
    TimestepChanged = false;
    TimestepMultiplier = 1.0f;
    MaximumAdjustment = 0.0f;
    Positions = 0;
    LastPositions = 0;
    CurrentTime = 0.0;
    SavedPositions = 0;
    GlobalDamping = 0.25f;
    Accelerations = 0;
    SimulationAlphas = 0;
    if (MemoryHandle != -1)
    {
        MemoryPool->FreeResource(MemoryHandle);
        MemoryHandle = -1;
    }
}
