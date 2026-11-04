// CParticleProgram::CParticleProgram (retail 0x00AE3550, Ego_r 0x00BF2B00): construct every member empty - the initial
// simulation resets itself, the program owns no particle memory yet - and Clear the program state.
#include "fable_particle_program.h"

// CParticleSimulation::CParticleSimulation (retail 0x00AE0600, Ego_r 0x00BEC210), defined first so the program
// constructor below expands it inline as retail does.
CParticleSimulation::CParticleSimulation()
    : MemoryHandle(-1), MemoryPool(0)
{
    Clear();
}

CParticleProgram::CParticleProgram()
    : MemoryHandle(-1), MemoryPool(0)
{
    Clear();
}
