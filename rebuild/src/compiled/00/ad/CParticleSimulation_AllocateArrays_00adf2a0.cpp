// CParticleSimulation::AllocateArrays (private; retail 0x00ADF2A0, Ego_r 0x00BEAF50): take one pool block for the
// simulation's per-particle arrays - positions, last and saved positions, accelerations (four-component vectors each)
// and the alphas - and point each array at its slice, in that order. The pool is remembered even when the allocation
// fails.
#include "fable_particle_simulation.h"
#include "fable_4d_vector.h"

bool CParticleSimulation::AllocateArrays(CMovableResourceMemoryPool& pool)
{
    MemoryPool = &pool;
    const unsigned long bytes = Size * (4 * sizeof(C4DVector) + sizeof(float));
    if (!pool.AllocateResource(&MemoryHandle, this, bytes, 0))
        return false;
    C4DVector* memory = static_cast<C4DVector*>(MemoryPool->GetPointerFromHandle(MemoryHandle));
    Positions = memory;
    memory += Size;
    LastPositions = memory;
    memory += Size;
    SavedPositions = memory;
    memory += Size;
    Accelerations = memory;
    memory += Size;
    SimulationAlphas = reinterpret_cast<float*>(memory);
    return true;
}
