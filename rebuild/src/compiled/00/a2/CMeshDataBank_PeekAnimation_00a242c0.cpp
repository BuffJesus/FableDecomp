#include "engine/CMeshDataBank.h"

const C3DAnimationInfo* CMeshDataBank::PeekAnimation(unsigned long index) const
{
    return reinterpret_cast<const C3DAnimationInfo*>(MeshesAndAnimations[index]);
}
