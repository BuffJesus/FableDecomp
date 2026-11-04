// CParticleProgram::GetUsedBones (retail 0x00AE1210 private static helper, 0x00AE15D0 public; Ego_r names): mark every bone
// that drives at least one vertex blend of the simulated or the non-simulated groups.
#include "fable_particle_program.h"

void FABLE_FASTCALL CParticleProgram::GetUsedBones(CArray<bool>& usedBones, const CArray<C3DGroup2>& groups)
{
    for (unsigned long i = 0; i < groups.size(); ++i)
    {
        if (groups[i].GetVertexBlendCount() > 0)
            usedBones[groups[i].GetBoneIndex()] = true;
    }
}

void CParticleProgram::GetUsedBones(CArray<bool>& usedBones) const
{
    GetUsedBones(usedBones, SimGroups);
    GetUsedBones(usedBones, NonSimGroups);
}
