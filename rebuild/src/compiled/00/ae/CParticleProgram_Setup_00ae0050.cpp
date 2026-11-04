// CParticleProgram::SetupSimulation (retail 0x00AE0050, Ego_r 0x00BEBB70): start a simulation as a copy of the
// program's initial simulation, its particle memory taken from the given pool.
#include "fable_particle_program.h"

bool CParticleProgram::SetupSimulation(CParticleSimulation& simulation, CMovableResourceMemoryPool& pool) const
{
    if (simulation.CopyFrom(InitialSimulation, pool))
        return true;
    return false;
}
