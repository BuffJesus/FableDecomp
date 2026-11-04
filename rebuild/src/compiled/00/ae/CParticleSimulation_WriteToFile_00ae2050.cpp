// CParticleSimulation::WriteToFile (retail 0x00AE2050, Ego_r 0x00BEEA60): the fields ReadFromFile reads, in the same
// order - timestep, changed flag (written as a ULONG), multiplier, particle count, the positions (WriteVectorArray), the
// simulation alphas as one block, then gravity, wind, dragging, acceleration and damping.
#include "fable_particle_simulation.h"
#include "fable_data_output_stream.h"

void CParticleSimulation::WriteToFile(CDataOutputStream& stream) const
{
    stream.WriteFloat(Timestep);
    stream.WriteULONG(TimestepChanged);
    stream.WriteFloat(TimestepMultiplier);
    stream.WriteULONG(Size);
    WriteVectorArray(Positions, Size, stream);
    stream.Write(SimulationAlphas, Size * sizeof(float));
    stream.WriteFloat(GravityStrength);
    stream.WriteFloat(WindStrength);
    stream.WriteEBOOL(DraggingEnable);
    stream.WriteEBOOL(DraggingRotational);
    stream.WriteFloat(DraggingStrength);
    stream.WriteEBOOL(AccelerationEnable);
    stream.WriteFloat(GlobalDamping);
}
