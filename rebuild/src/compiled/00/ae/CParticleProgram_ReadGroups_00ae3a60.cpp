// CParticleProgram::ReadGroups (private static; retail 0x00AE3A60, Ego_r name): read a group list - the count, then per
// group its bone index, its blend count and the blends as one raw block - as ReadFromFile does for both group lists.
// Retail calls vector<C3DGroup2>::resize(n) out of line (0x00AE8540).
#define FABLE_VECTOR_RESIZE_OUT_OF_LINE
#include "fable_particle_program.h"
#include "fable_data_input_stream.h"

void FABLE_FASTCALL CParticleProgram::ReadGroups(CDataInputStream& stream, CArray<C3DGroup2>& groups)
{
    unsigned long count = stream.ReadULONG();
    groups.resize(count);
    for (unsigned long i = 0; i < count; ++i)
    {
        C3DGroup2& group = groups[i];
        group.SetBoneIndex(stream.ReadValue<long>());
        unsigned long blends = stream.ReadULONG();
        group.Resize(blends);
        stream.Read(group.GetVertexBlends(), blends * sizeof(C3DVertexBlend2));
    }
}
