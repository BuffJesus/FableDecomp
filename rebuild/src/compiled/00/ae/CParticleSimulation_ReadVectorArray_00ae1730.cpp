// CParticleSimulation::ReadVectorArray (protected static; retail 0x00AE1730, Ego_r 0x00BEE140): read count stored
// three-component vectors in one block into a temporary array, then widen each into the four-component destination
// with the given W.
#include "fable_particle_simulation.h"
#include "fable_data_input_stream.h"
#include "fable_carray.h"
#include "fable_2d_vector.h"
#include "fable_4d_vector.h"

void FABLE_FASTCALL CParticleSimulation::ReadVectorArray(C4DVector* vectors, unsigned long count, CDataInputStream& stream, float w)
{
    CArray<C3DVector> stored(count);
    stream.Read(stored.GetAsCArray(), stored.size() * sizeof(C3DVector));
    for (unsigned long i = 0; i < count; ++i)
    {
        const C3DVector& in = stored[i];
        C4DVector& out = vectors[i];
        out.X = in.X;
        out.Y = in.Y;
        out.Z = in.Z;
        out.W = w;
    }
}
