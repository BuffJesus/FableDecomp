// MeshDataBank.cpp accessors, retail 0x00A23900..0x00A23DFF, in Ego_r.pdb MeshDataBank.cpp module
// order (b5fca0..b60250). Two members of the same run have their own per-address units:
// C3DAnimationInfo::IsLoading 0x00A23F30 (C3DAnimationInfo_IsLoading_00a23f30.cpp) and
// CMeshDataBank::FreeDeviceSpecificResources 0x00A24000
// (CMeshDataBank_FreeDeviceSpecificResources_00a24000.cpp). Owners: retail RTTI vtables (CMeshDataBankEntry 0x0129CD28, C3DMeshInfo
// 0x0129CD3C, C3DMeshLODInfo 0x0129CDA4, CMeshDataBank 0x0129CE94) and retail callers.
#include "engine/CMeshDataBank.h"

// CMeshDataBankEntry vtable slot 1; inherited unchanged by CPhysicsMeshInfo (0x0129CD50) and
// C3DAnimationInfo (0x0129CD64), whose class hierarchies list CMeshDataBankEntry as their base.
void CMeshDataBankEntry::FreeDeviceSpecificResources()
{
}

// CMeshDataBank vtable slot 8 (CBankFile::GetDataFormatVersion is pure).
unsigned long CMeshDataBank::GetDataFormatVersion()
{
    return 205;
}

// Callers 0x00B3CD13 / 0x00B3CE71 (only when no texture bank is set yet).
void CMeshDataBank::SetTextureBank(const CGraphicDataBank* textureBank)
{
    TextureBank = textureBank;
}

// Callers 0x00B321A6 / 0x00B34711.
void CMeshDataBank::SetDP3BumpEnable(bool enable)
{
    DP3BumpEnable = enable;
}

// C3DMeshLODInfo vtable slot 6.
bool C3DMeshLODInfo::CanAbortResourceLoad(long priority) const
{
    return priority > 1;
}

// Callers 0x006B05D1 / 0x0072408E on PeekStaticMesh / PeekAnimatedMesh results.
bool C3DMeshInfo::HasPhysicsMesh() const
{
    return PhysicsMeshIndex != -1;
}

// Eight callers pass a load priority after C3DMeshInfo 0x00A256F0; the PC build streams no
// textures here.
void C3DMeshInfo::BeginLoadingTextures(EBankLoadPriority) const
{
}

// Callers 0x00B9CE1E / 0x00B9EADC / 0x00BD2102 test it after IsLoaded 0x00A23E40.
bool C3DMeshInfo::AreTexturesLoaded() const
{
    return true;
}

// C3DMeshInfo vtable slot 1.
void C3DMeshInfo::FreeDeviceSpecificResources()
{
}

// Callers (e.g. 0x00856DE8, 0x008E90AC) apply it to PeekAnimation results.
float C3DAnimationInfo::GetNonLoopingDuration() const
{
    return NonLoopingDuration;
}

// C3DMeshLODInfo vtable slot 4: a pending file read answers for itself
// (CBankFileAsyncData::IsFinished 0x009D54E0, a tail jump).
bool C3DMeshLODInfo::IsResourceLoaded() const
{
    if (FileData.Data)
        return FileData.Data->IsFinished();
    return true;
}
