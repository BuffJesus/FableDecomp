// CParticleProgram::SaveStateToInterpolateFrom (retail 0x00ADF5B0, Ego_r 0x00BEB250): remember the simulation's
// current particle positions as the state the next step interpolates from. The program itself is not read.
#include <string.h>
#include "fable_particle_program.h"

void CParticleProgram::SaveStateToInterpolateFrom(CParticleSimulation& simulation)
{
    memcpy(simulation.SavedPositions, simulation.Positions, simulation.Size * sizeof(C4DVector));
}
