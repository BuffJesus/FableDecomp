// CParticleSimulation::RelocateData (retail 0x00ADFC60, vtable slot 1; Ego_r name): the particle arrays live in one
// movable-pool block; when the pool moves it, shift each array pointer by the same distance.
#include "fable_particle_simulation.h"

template <class T>
static inline void Relocate(T*& pointer, void* oldAddress, void* newAddress)
{
    if (pointer)
        pointer = (T*)((char*)pointer - (char*)oldAddress + (char*)newAddress);
}

void CParticleSimulation::RelocateData(long, void* oldAddress, void* newAddress, long)
{
    Relocate(Positions, oldAddress, newAddress);
    Relocate(LastPositions, oldAddress, newAddress);
    Relocate(SavedPositions, oldAddress, newAddress);
    Relocate(Accelerations, oldAddress, newAddress);
    Relocate(SimulationAlphas, oldAddress, newAddress);
}
