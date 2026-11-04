// CParticleProgram::AddGroup (retail 0x00AE37D0, Ego_r 0x00BF3730): split a mesh bone group between the simulated and the
// non-simulated vertex sets. Each blend whose source vertex became a particle goes to the new SimGroups entry (re-indexed
// to the particle); each whose vertex is still rendered directly goes to the new NonSimGroups entry; a vertex can be both.
#include <new>
#include "fable_particle_program.h"

void CParticleProgram::AddGroup(const C3DGroup2& group)
{
    SimGroups.push_back(C3DGroup2());
    NonSimGroups.push_back(C3DGroup2());
    C3DGroup2& simGroup = SimGroups.back();
    C3DGroup2& nonSimGroup = NonSimGroups.back();
    simGroup.SetBoneIndex(group.GetBoneIndex());
    nonSimGroup.SetBoneIndex(group.GetBoneIndex());
    for (unsigned long i = 0; i < (unsigned long)group.GetVertexBlendCount(); ++i)
    {
        const C3DVertexBlend2& blend = group.PeekVertexBlend(i);
        unsigned long vertex = blend.GetVertexIndex();
        float weight = blend.GetBlendValue();
        if (vertex < ParticleIndices.size() && ParticleIndices[vertex] != -1)
            simGroup.AddVertexBlend(C3DVertexBlend2(ParticleIndices[vertex], weight));
        if (vertex < VertexIndices.size() && VertexIndices[vertex] != -1)
            nonSimGroup.AddVertexBlend(C3DVertexBlend2(VertexIndices[vertex], weight));
    }
}
