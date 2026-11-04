// CParticleProgram::RelocateData (retail 0x00AE04C0, vtable slot 1; Ego_r name): the non-simulated positions live in a
// movable-pool block; when the pool moves it, shift the pointer by the same distance.
#include "fable_particle_program.h"

void CParticleProgram::RelocateData(long, void* oldAddress, void* newAddress, long)
{
    if (NonSimPositions)
        NonSimPositions = (C4DVector*)((char*)NonSimPositions - (char*)oldAddress + (char*)newAddress);
}
