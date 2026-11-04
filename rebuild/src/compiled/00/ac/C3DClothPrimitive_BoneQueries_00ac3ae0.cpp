// C3DClothPrimitive bone queries (retail 0x00AC3AE0 GetUsedBones, 0x00AC3AF0 RemapBoneIndices; Ego_r 0x00C4B600 /
// 0x00C4B610): the cloth's bones are the particle program's, so both forward to it (C3DPrimitive2::GetUsedBones
// 0x00A8FCF0 and ::RemapBoneIndices 0x00A90F00 call these for each cloth primitive).
#include "fable_3d_cloth_primitive.h"

void C3DClothPrimitive::GetUsedBones(CArray<bool>& usedBones) const
{
    ParticleProgram->GetUsedBones(usedBones);
}

void C3DClothPrimitive::RemapBoneIndices(_STL::map<unsigned long, unsigned long>& boneRemap)
{
    ParticleProgram->RemapBoneIndices(boneRemap);
}
