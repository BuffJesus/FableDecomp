// CParticleProgram::WriteGroups (private static; retail 0x00AE0680, Ego_r 0x00BEC770): the inverse of ReadGroups - the
// group count, then per group its bone index, its blend count and the blends as one raw block.
#include "fable_particle_program.h"
#include "fable_data_output_stream.h"

void FABLE_FASTCALL CParticleProgram::WriteGroups(CDataOutputStream& stream, const CArray<C3DGroup2>& groups)
{
    unsigned long count = groups.size();
    stream.WriteULONG(count);
    for (unsigned long i = 0; i < count; ++i)
    {
        const C3DGroup2& group = groups[i];
        stream.WriteULONG(group.GetBoneIndex());
        unsigned long blends = group.GetVertexBlendCount();
        stream.WriteULONG(blends);
        stream.Write(group.PeekVertexBlends(), blends * sizeof(C3DVertexBlend2));
    }
}
