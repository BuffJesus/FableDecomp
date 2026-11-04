// CParticleSimulation::WriteVectorArray (protected static; retail 0x00AE18C0, Ego_r 0x00BEE2B0): the inverse of
// ReadVectorArray - narrow count four-component vectors to C3DVector in a temporary array and write it as one block.
#include "fable_particle_simulation.h"
#include "fable_data_output_stream.h"
#include "fable_carray.h"
#include "fable_2d_vector.h"
#include "fable_4d_vector.h"

void FABLE_FASTCALL CParticleSimulation::WriteVectorArray(const C4DVector* vectors, unsigned long count, CDataOutputStream& stream)
{
    CArray<C3DVector> stored(count);
    for (unsigned long i = 0; i < count; ++i)
        stored[i] = vectors[i].ToC3DVector();
    stream.Write(stored.GetAsCArray(), stored.size() * sizeof(C3DVector));
}
