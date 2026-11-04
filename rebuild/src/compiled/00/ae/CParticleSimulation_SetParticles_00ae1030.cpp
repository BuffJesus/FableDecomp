// CParticleSimulation::SetParticles (retail 0x00AE1030, Ego_r 0x00BEC2F0) and CParticleProgram::SetData (retail
// 0x00AE1150, Ego_r 0x00BEC670): fill a simulation / a program from authored vertex data. SetParticles makes one
// particle per position (w = 0) at rest - current, previous and saved positions equal, no acceleration - with its
// simulation alpha; the allocation result is not checked. SetData copies the non-simulated vertices (w = 0) into the
// program's pool memory and reports whether the allocation succeeded.
#include "fable_particle_program.h"
#include "fable_4d_vector.h"

void CParticleSimulation::SetParticles(const CArray<C3DVector>& positions, const CArray<float>& alphas,
                                       CMovableResourceMemoryPool& pool)
{
    Size = positions.size();
    AllocateArrays(pool);
    for (unsigned long i = 0; i < Size; ++i)
    {
        C4DVector position(positions[i], 0.0f);
        Positions[i] = position;
        LastPositions[i] = position;
        SavedPositions[i] = position;
        Accelerations[i] = C4DVector(0.0f, 0.0f, 0.0f, 0.0f);
        SimulationAlphas[i] = alphas[i];
    }
}

bool CParticleProgram::SetData(const CArray<C3DVector>& vertices, CMovableResourceMemoryPool& pool)
{
    NonSimCount = vertices.size();
    if (!AllocateArrays(NonSimCount, pool))
        return false;
    for (unsigned long i = 0; i < NonSimCount; ++i)
        NonSimPositions[i] = C4DVector(vertices[i], 0.0f);
    return true;
}
