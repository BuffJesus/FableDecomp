// C3DGroup2::SetBoneIndex (retail 0x00AC4070, Ego_r name): move the group to another bone (CParticleProgram's bone remap).
#include "fable_3d_group2.h"

void C3DGroup2::SetBoneIndex(long index)
{
    BoneIndex = index;
}
