// CParticleProgram::PrepareForRendering overloads (retail 0x00AE1260 skinned, 0x00AE1600 static; Ego_r 0x00BECF30 /
// 0x00BECE70). The render array holds the NonSimCount non-simulated vertices first, then one entry per simulated
// particle: its position interpolated between the previous step (SavedPositions) and the current one by alpha. When
// blend is above DELTA_NOTIONAL_ZERO those particles are further pulled towards the initial simulation's positions by
// blend. The skinned overload transforms the non-simulated vertices (and, for the blend, the initial positions, into the
// shared vector scratch buffer) by the bone matrices of their groups first; its animation arguments are not read here.
// Retail expands the static overload's STLport copy of the non-simulated vertices inline (FABLE_UNIT_INLINE, rebuild_abi.h).
#define FABLE_UNIT_INLINE __forceinline
#include "fable_particle_program.h"
#include "fable_vector_math.h"
#include "fable_stlport_algobase.h"

void CParticleProgram::PrepareForRendering(C4DVector* positions, CParticleSimulation& simulation, float alpha, float blend,
                                           const C3DAnimationBlendState& blendState,
                                           const C3DAnimationBlendState& previousBlendState, float blendFraction,
                                           C3DAnimationManager& animationManager, const C3DVector* vertices,
                                           const CMatrix4x4* matrices, const C3DMesh2& mesh)
{
    TransformVertices(positions, NonSimPositions, NonSimCount, matrices, NonSimGroups);
    NVectorMath::GFInterpolateVectors(positions + NonSimCount, sizeof(C4DVector), simulation.SavedPositions,
                                      sizeof(C4DVector), simulation.Positions, sizeof(C4DVector), simulation.Size, alpha);
    if (blend > DELTA_NOTIONAL_ZERO)
    {
        C4DVector* initial = GetVectorBuffer(simulation.Size);
        TransformVertices(initial, InitialSimulation.Positions, simulation.Size, matrices, SimGroups);
        NVectorMath::GFInterpolateVectors(positions + NonSimCount, sizeof(C4DVector), initial, sizeof(C4DVector),
                                          simulation.Size, blend);
    }
}

void CParticleProgram::PrepareForRendering(C4DVector* positions, CParticleSimulation& simulation, float alpha, float blend)
{
    _STL::copy(NonSimPositions, NonSimPositions + NonSimCount, positions);
    NVectorMath::GFInterpolateVectors(positions + NonSimCount, sizeof(C4DVector), simulation.SavedPositions,
                                      sizeof(C4DVector), simulation.Positions, sizeof(C4DVector), simulation.Size, alpha);
    if (blend > DELTA_NOTIONAL_ZERO)
        NVectorMath::GFInterpolateVectors(positions + NonSimCount, sizeof(C4DVector), InitialSimulation.Positions,
                                          sizeof(C4DVector), simulation.Size, blend);
}
