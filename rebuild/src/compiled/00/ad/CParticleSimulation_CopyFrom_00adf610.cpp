// CParticleSimulation::CopyFrom (retail 0x00ADF610, Ego_r 0x00BEB340): become a copy of another simulation. The timestep
// state and particle count are copied first; then this simulation's arrays are allocated from the pool (on failure
// nothing more is copied and false is returned), every particle array is copied, and the remaining settings, clock and
// bounding box follow. MaximumAdjustment is not copied. CParticleSimulation::GetBoundingBox (retail 0x00ADF730, Ego_r
// 0x00BEB440): return the bounding box and mark it valid.
#include <string.h>
#include "fable_particle_simulation.h"
#include "fable_4d_vector.h"

bool CParticleSimulation::CopyFrom(const CParticleSimulation& source, CMovableResourceMemoryPool& pool)
{
    Timestep = source.Timestep;
    TimestepChanged = source.TimestepChanged;
    TimestepMultiplier = source.TimestepMultiplier;
    Size = source.Size;
    if (!AllocateArrays(pool))
        return false;
    memcpy(Positions, source.Positions, Size * sizeof(C4DVector));
    memcpy(LastPositions, source.LastPositions, Size * sizeof(C4DVector));
    memcpy(SavedPositions, source.SavedPositions, Size * sizeof(C4DVector));
    memcpy(Accelerations, source.Accelerations, Size * sizeof(C4DVector));
    memcpy(SimulationAlphas, source.SimulationAlphas, Size * sizeof(float));
    GravityStrength = source.GravityStrength;
    WindStrength = source.WindStrength;
    DraggingEnable = source.DraggingEnable;
    DraggingRotational = source.DraggingRotational;
    DraggingStrength = source.DraggingStrength;
    AccelerationEnable = source.AccelerationEnable;
    GlobalDamping = source.GlobalDamping;
    CurrentTime = source.CurrentTime;
    BoundingBox = source.BoundingBox;
    BoundingBoxValid = source.BoundingBoxValid;
    return true;
}

C3DBoundingBox CParticleSimulation::GetBoundingBox() const
{
    BoundingBoxValid = true;
    return BoundingBox;
}
