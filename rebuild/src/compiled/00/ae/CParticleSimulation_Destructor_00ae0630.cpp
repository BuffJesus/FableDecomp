// CParticleSimulation::~CParticleSimulation (retail 0x00AE0630, Ego_r 0x00BEC260): Clear (which hands the particle
// memory back to its pool), then the render-data counted pointer and CMovableResource bases unwind inline.
// CParticleProgram::~CParticleProgram (retail 0x00AE3670, Ego_r 0x00BF2C50): Clear, then every member is destroyed;
// retail expands the initial simulation's destructor above inline, so it must follow it in this unit.
#include "fable_particle_simulation.h"
#include "fable_particle_program.h"

CParticleSimulation::~CParticleSimulation()
{
    Clear();
}

CParticleProgram::~CParticleProgram()
{
    Clear();
}
