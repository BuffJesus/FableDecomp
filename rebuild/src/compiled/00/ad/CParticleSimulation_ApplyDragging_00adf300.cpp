// CParticleSimulation::ApplyDragging (private; retail 0x00ADF300, Ego_r 0x00BEAFB0). When dragging is enabled, take
// the owner's movement since the last step from the context, blend it towards the identity by 1 - DraggingStrength,
// and move both the current and the previous particle positions by the result (so the cloth follows its owner).
#include "fable_particle_simulation.h"
#include "fable_particle_context.h"
#include "fable_matrix4x4.h"
#include "fable_4d_vector.h"

void CParticleSimulation::ApplyDragging(const CParticleContext& context)
{
    if (DraggingEnable)
    {
        CMatrix3x4 delta;
        context.GetDeltaTransform(delta);
        __declspec(align(16)) CMatrix4x4 transform(delta);
        transform.AlignedSetInterpolate(CMatrix4x4::Identity, 1.0f - DraggingStrength);
        transform.AlignedTransformTransposedAssumeW(Positions, Size, 1.0f);
        transform.AlignedTransformTransposedAssumeW(LastPositions, Size, 1.0f);
    }
}
