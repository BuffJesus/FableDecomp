// C3DGroup2 default constructor (retail 0x00AC40C0) and AddVertexBlend (0x00AC4110); Ego_r names. A new group belongs to
// no bone until CParticleProgram::AddGroup assigns one, and blends are appended in order.
#include <new>
#include "fable_chunked_file.h"
#include "fable_stlport_vector_impl.h"
#include "fable_3d_group2.h"

C3DGroup2::C3DGroup2()
    : BoneIndex(-1)
{
}

void C3DGroup2::AddVertexBlend(const C3DVertexBlend2& blend)
{
    VertexBlends.push_back(blend);
}
