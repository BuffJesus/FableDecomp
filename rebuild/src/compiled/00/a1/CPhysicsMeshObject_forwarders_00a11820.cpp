// CPhysicsMeshObject forwarders onto the shared CPhysicsMesh (Ego_r CPhysicsMeshObject members;
// retail order GetNoHelperPoints 0xA11750, GetNoDummyObjects 0xA11800, GetAllDummyObjectNames
// 0xA11810, GetAllHelperPointNames 0xA11820, ..., GetNumberOfNavigationLineLevels 0xA127C0,
// ~CPhysicsMeshObject 0xA127D0, ..., IsPointContainedInBoundingBox 0xA12890).
#include "fable_physics_mesh.h"

void CPhysicsMeshObject::GetAllHelperPointNames(_STL::vector<CCharString>& names) const
{
    Mesh->GetAllHelperPointNames(names);
}

long CPhysicsMeshObject::GetNumberOfNavigationLineLevels() const
{
    return Mesh->GetNumberOfNavigationLineLevels();
}

bool CPhysicsMeshObject::IsPointContainedInBoundingBox(const C3DVector& point) const
{
    return IsSpherePartContainedInBoundingBox(point, 0.0f);
}
