// CParticleSimulation::ReadFromFile (retail 0x00AE1BC0, Ego_r 0x00BEE620): load a simulation - timestep settings and
// particle count, then (once the pool has room for the arrays) the particle positions, copied as the last and saved
// positions too, the per-particle alphas, the forces and dragging settings and the damping. The bounding box is stale.
#include "fable_particle_simulation.h"
#include "fable_data_input_stream.h"
#include "fable_4d_vector.h"
#include "fable_stlport_algobase.h"

bool CParticleSimulation::ReadFromFile(CDataInputStream& stream, CMovableResourceMemoryPool& pool)
{
    Timestep = stream.ReadFloat();
    TimestepChanged = stream.ReadEBOOL();
    TimestepMultiplier = stream.ReadFloat();
    Size = stream.ReadULONG();
    if (!AllocateArrays(pool))
        return false;
    ReadVectorArray(Positions, Size, stream, 0.0f);
    _STL::copy(Positions, Positions + Size, LastPositions);
    _STL::copy(Positions, Positions + Size, SavedPositions);
    stream.Read(SimulationAlphas, Size * sizeof(float));
    GravityStrength = stream.ReadFloat();
    WindStrength = stream.ReadFloat();
    DraggingEnable = stream.ReadEBOOL();
    DraggingRotational = stream.ReadEBOOL();
    DraggingStrength = stream.ReadFloat();
    AccelerationEnable = stream.ReadEBOOL();
    GlobalDamping = stream.ReadFloat();
    BoundingBoxValid = false;
    return true;
}
