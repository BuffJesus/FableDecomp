// CParticleSimulation::ApplyForces (private; retail 0x00ADF760, Ego_r 0x00BEC410): one Verlet integration step. With
// acceleration enabled: gravity and wind from the context, scaled by their strengths, become the acceleration; the
// current and previous position arrays swap roles; each particle moves to current + (current - previous) * damping +
// acceleration * Timestep^2, where damping = (1 - GlobalDamping)^Timestep * TimestepMultiplier. Without acceleration the
// current positions are only copied to the previous ones. Either way the timestep is no longer "changed" and its
// multiplier returns to 1.
#include <math.h>
#include <string.h>
#include "fable_particle_simulation.h"
#include "fable_particle_context.h"
#include "fable_4d_vector.h"

void CParticleSimulation::ApplyForces(const CParticleContext& context)
{
    if (AccelerationEnable)
    {
        C3DVector gravity = context.GetGravityVector();
        C3DVector wind = context.GetWindVector();
        C3DVector acceleration = gravity * GravityStrength + wind * WindStrength;
        C4DVector* previous = LastPositions;
        LastPositions = Positions;
        Positions = previous;
        float damping = pow(1.0f - GlobalDamping, Timestep) * TimestepMultiplier;
        float timestepSquared = Timestep * Timestep;
        __declspec(align(16)) C4DVector step = C4DVector(acceleration, 0.0f) * timestepSquared;
        for (unsigned long i = 0; i < Size; ++i)
            Positions[i] = LastPositions[i] + (LastPositions[i] - Positions[i]) * damping + step;
    }
    else
        memcpy(LastPositions, Positions, Size * sizeof(C4DVector));
    TimestepChanged = false;
    TimestepMultiplier = 1.0f;
}
