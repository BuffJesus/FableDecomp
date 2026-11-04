// CParticleProgram::ExecuteProgram (private; retail 0x00AE1A90, Ego_r 0x00BEDB00) and ApplyConstraints (private; retail
// 0x00AE2510, Ego_r 0x00BEE480). The compiled constraint program is a stream of records - type, count, payload byte size
// (three dwords), then the payload. REPEAT opens a loop that resumes after its own record, END_REPEAT jumps back until its
// count runs out, DISTANCE records are solved by the simulation; the min/max-distance, unbend and sphere-collision
// solvers are empty in this build (defined here first so the program inlines them away, as retail does).
// ApplyConstraints runs the program until a pass moves no particle by more than maximumDistanceError, at most
// maximumIterations times.
// Retail expands LoopStack's clear (STLport copy) and push_back here: FABLE_UNIT_INLINE (rebuild_abi.h) and the
// vector implementation header.
#define FABLE_UNIT_INLINE __forceinline
#include <new>
#include "fable_particle_program.h"
#include "fable_stlport_algobase.h"
#include "fable_stlport_vector_impl.h"

void CParticleSimulation::SatisfyMinMaxDistanceConstraints(const CParticleMinMaxDistanceConstraint*, unsigned long)
{
}

void CParticleSimulation::SatisfySphereCollisionConstraints(const CParticleSphereCollisionConstraint*, unsigned long)
{
}

void CParticleSimulation::SatisfyUnbendConstraints(const CParticleUnbendConstraint*, unsigned long)
{
}

void CParticleProgram::ExecuteProgram(CParticleSimulation& simulation) const
{
    const unsigned char* program = static_cast<const unsigned char*>(Constraints.PeekData());
    unsigned long length = Constraints.GetLength();
    LoopStack.clear();
    for (unsigned long position = 0; position < length;)
    {
        const unsigned long* record = reinterpret_cast<const unsigned long*>(program + position);
        unsigned long count = record[1];
        unsigned long size = record[2];
        TConstraintType type = static_cast<TConstraintType>(record[0]);
        position += 3 * sizeof(unsigned long);
        switch (type)
        {
        case CONSTRAINT_REPEAT:
        {
            CLoopBegin loop;
            loop.Position = position + size;
            loop.LoopCount = count;
            LoopStack.push_back(loop);
            break;
        }
        case CONSTRAINT_END_REPEAT:
            if (!LoopStack.empty())
            {
                CLoopBegin& loop = LoopStack.back();
                if (--loop.LoopCount > 0)
                    position = loop.Position;
                else
                    LoopStack.pop_back();
            }
            break;
        case CONSTRAINT_DISTANCE:
            simulation.SatisfyDistanceConstraints(reinterpret_cast<const CParticleDistanceConstraint*>(program + position), count);
            break;
        case CONSTRAINT_MIN_MAX_DISTANCE:
            simulation.SatisfyMinMaxDistanceConstraints(reinterpret_cast<const CParticleMinMaxDistanceConstraint*>(program + position), count);
            break;
        case CONSTRAINT_UNBEND:
            simulation.SatisfyUnbendConstraints(reinterpret_cast<const CParticleUnbendConstraint*>(program + position), count);
            break;
        case CONSTRAINT_SPHERE_COLLISION:
            simulation.SatisfySphereCollisionConstraints(reinterpret_cast<const CParticleSphereCollisionConstraint*>(program + position), count);
            break;
        }
        position += size;
    }
}

void CParticleProgram::ApplyConstraints(CParticleSimulation& simulation, long maximumIterations, float maximumDistanceError) const
{
    for (long i = 0; i < maximumIterations; ++i)
    {
        simulation.ResetMaximumAdjustment();
        ExecuteProgram(simulation);
        if (simulation.GetMaximumAdjustment() <= maximumDistanceError)
            break;
    }
}
