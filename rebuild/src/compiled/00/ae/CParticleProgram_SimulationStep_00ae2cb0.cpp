// CParticleProgram::SimulationStep overloads (retail 0x00AE2CB0 / 0x00AE2D20, Ego_r 0x00BEF1A0 / 0x00BEF210). An empty
// simulation is left alone. Otherwise: apply dragging and forces, then either pull each particle back towards the
// initial simulation's position by its simulation alpha (free-standing cloth) or cling to the animated mesh, and finally
// run the constraint program with the context's iteration and distance-error limits.
#include "fable_particle_program.h"
#include "fable_particle_context.h"
#include "fable_vector_math.h"

void CParticleProgram::SimulationStep(CParticleSimulation& simulation, const CParticleContext& context) const
{
    if (simulation.Size)
    {
        simulation.ApplyDragging(context);
        simulation.ApplyForces(context);
        NVectorMath::GFInterpolateVectors(simulation.Positions, sizeof(C4DVector), InitialSimulation.Positions,
                                          sizeof(C4DVector), simulation.Positions, sizeof(C4DVector),
                                          simulation.SimulationAlphas, sizeof(float), InitialSimulation.Size);
        ApplyConstraints(simulation, context.GetMaximumIterations(), context.GetMaximumDistanceError());
    }
}

void CParticleProgram::SimulationStep(CParticleSimulation& simulation, const CParticleContext& context,
                                      const C3DAnimationBlendState& blendState, C3DAnimationManager& animationManager,
                                      const C3DVector* vertices, const C3DMesh2& mesh) const
{
    if (simulation.Size)
    {
        simulation.ApplyDragging(context);
        simulation.ApplyForces(context);
        ClingToMesh(simulation, blendState, animationManager, vertices, mesh);
        ApplyConstraints(simulation, context.GetMaximumIterations(), context.GetMaximumDistanceError());
    }
}
