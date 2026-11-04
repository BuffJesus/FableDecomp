// CParticleProgram::AllocateArrays (private; retail 0x00ADF460, Ego_r 0x00BEB110): take one pool block of count
// four-component positions for the program's particles. Refuses a count whose byte size would not fit 32 bits; the pool
// is remembered either way.
#include "fable_particle_program.h"

bool CParticleProgram::AllocateArrays(unsigned long count, CMovableResourceMemoryPool& pool)
{
    MemoryPool = &pool;
    if ((unsigned __int64)count * sizeof(C4DVector) > 0xFFFFFFFFu)
        return false;
    if (!pool.AllocateResource(&MemoryHandle, this, count * sizeof(C4DVector), 0))
        return false;
    NonSimPositions = static_cast<C4DVector*>(MemoryPool->GetPointerFromHandle(MemoryHandle));
    return true;
}
