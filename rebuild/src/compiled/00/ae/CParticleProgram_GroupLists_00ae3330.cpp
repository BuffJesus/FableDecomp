// CParticleProgram group-list wrappers (retail 0x00AE3330 ReserveGroups, 0x00AE3A30 RemapBoneIndices; Ego_r names):
// each applies the same step to the simulated and the non-simulated group lists. GetUsedBones 0x00AE15D0 has the
// same shape but retail reuses ecx across its two helper calls (the helper 0x00AE1210 shares the TU); not landed yet.
#include "fable_particle_program.h"

void CParticleProgram::ReserveGroups(unsigned long count)
{
    SimGroups.reserve(count);
    NonSimGroups.reserve(count);
}

void CParticleProgram::RemapBoneIndices(_STL::map<unsigned long, unsigned long>& boneRemap)
{
    RemapBoneIndices(boneRemap, SimGroups);
    RemapBoneIndices(boneRemap, NonSimGroups);
}
