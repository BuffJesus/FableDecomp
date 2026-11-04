// CParticleProgram::Update (retail 0x00AE34D0, Ego_r 0x00BEF280): sort the exported particle names if any were added,
// then recompute the average patch size from the initial simulation. CParticleProgram::ClearGroups (retail 0x00AE3510,
// Ego_r 0x00BEF4A0): empty the simulated and non-simulated group lists.
#include "fable_particle_program.h"

void CParticleProgram::Update()
{
    ExportParticles.Update();
    CalculateAveragePatchSize(InitialSimulation);
}

void CParticleProgram::ClearGroups()
{
    SimGroups.clear();
    NonSimGroups.clear();
}
