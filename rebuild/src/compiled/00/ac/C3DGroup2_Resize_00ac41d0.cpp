// C3DGroup2::Resize (retail 0x00AC41D0, Ego_r name): size the vertex blend list (CParticleProgram::ReadGroups then fills it
// with one raw read).
#include <new>
#include "fable_chunked_file.h"
#include "fable_stlport_vector_impl.h"   // retail expands STLport resize(n, x) here
#include "fable_3d_group2.h"

void C3DGroup2::Resize(unsigned long count)
{
    VertexBlends.resize(count);
}
