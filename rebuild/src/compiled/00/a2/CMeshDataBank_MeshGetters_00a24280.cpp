#include "engine/CMeshDataBank.h"

const C3DMeshInfo* CMeshDataBank::PeekStaticMesh(unsigned long index) const
{
    return reinterpret_cast<const C3DMeshInfo*>(MeshesAndAnimations[index]);
}

const C3DMeshInfo* CMeshDataBank::PeekRepeatedMesh(unsigned long index) const
{
    return reinterpret_cast<const C3DMeshInfo*>(MeshesAndAnimations[index]);
}
