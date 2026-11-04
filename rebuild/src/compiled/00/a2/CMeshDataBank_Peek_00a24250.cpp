// CMeshDataBank entry getters, retail 0x00A24250..0x00A242B0 (Ego_r MeshDataBank.cpp order
// b60500..b60570; PeekStaticMesh 0x00A24280, PeekRepeatedMesh 0x00A24290 and PeekAnimation
// 0x00A242C0 are landed in CMeshDataBank_MeshGetters_00a24280.cpp / CMeshDataBank_PeekAnimation_00a242c0.cpp),
// and C3DMeshInfo::PeekPhysicsMesh 0x00A24A70. FableWin 0x0313D78A calls the protected
// CMeshDataBankEntry::GetBank() and then PeekPhysicsMeshInfo (after ASSERT(HasPhysicsMesh()),
// compiled out of retail); /O2 expands both (Bank->MeshesAndAnimations[PhysicsMeshIndex]). The manifest's CTCInventoryAbilities labels
// on 0x00A24250..0x00A242B0 are propagated names.
#include "engine/CMeshDataBank.h"

const CPhysicsMeshInfo* CMeshDataBank::PeekPhysicsMeshInfo(unsigned long index) const
{
    return reinterpret_cast<const CPhysicsMeshInfo*>(MeshesAndAnimations[index]);
}

const C3DMeshInfo* CMeshDataBank::PeekMeshInfo(unsigned long index) const
{
    return reinterpret_cast<const C3DMeshInfo*>(MeshesAndAnimations[index]);
}

const C3DMeshInfo* CMeshDataBank::PeekMesh(unsigned long index) const
{
    return reinterpret_cast<const C3DMeshInfo*>(MeshesAndAnimations[index]);
}

const C3DMeshInfo* CMeshDataBank::PeekParticleMesh(unsigned long index) const
{
    return reinterpret_cast<const C3DMeshInfo*>(MeshesAndAnimations[index]);
}

const C3DMeshInfo* CMeshDataBank::PeekAnimatedMesh(unsigned long index) const
{
    return reinterpret_cast<const C3DMeshInfo*>(MeshesAndAnimations[index]);
}

const CPhysicsMeshInfo* C3DMeshInfo::PeekPhysicsMesh() const
{
    return GetBank()->PeekPhysicsMeshInfo(PhysicsMeshIndex);
}
