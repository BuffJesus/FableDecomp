#pragma once

// CPhysicsMesh / CPhysicsMeshObject as the retail physics-mesh-object TU
// (0x00A11750..0x00A13BE0) sees them. Names: Ego_r.pdb.
// Ancestry (retail RTTI): CPhysicsMesh : CIVCountedPointeeBase (vtable 0x0129DC68);
// CPhysicsMeshObject : CBaseClass (vtable 0x0125D084, slot 0 = vector deleting
// destructor 0x006A6BA0; destructor 0x00A127D0).
// Kept at the include root (not engine/) so gen_class_headers.py treats both
// classes as hand-owned.

#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_base_class.h"
#include "fable_iv_counted_pointer.h"
#include "fable_stlport_vector.h"
#include "engine/CMatrix3x4.h"

class CCharString;
class C3DVector;

// Ego_r.pdb CPhysicsMesh (the Mesh shared by every CPhysicsMeshObject). Only the
// members the reconstructed CPhysicsMeshObject forwarders call are declared.
class CPhysicsMesh : public CIVCountedPointeeBase
{
public:
    // Ego_r ?GetAllHelperPointNames@CPhysicsMesh@@QBEXAAV?$vector@VCCharString@@...@Z, retail 0x00A6D050.
    void GetAllHelperPointNames(_STL::vector<CCharString>& names) const;
    // Ego_r ?GetAllDummyObjectNames@CPhysicsMesh@@QBEXAAV?$vector@VCCharString@@...@Z, retail 0x00A6D090.
    void GetAllDummyObjectNames(_STL::vector<CCharString>& names) const;
    // Ego_r ?GetNumberOfNavigationLineLevels@CPhysicsMesh@@QBEJXZ, retail 0x00A73CF0.
    long GetNumberOfNavigationLineLevels() const;
};

// Ego_r.pdb CPhysicsMeshObject (0x40 bytes): vptr, Mesh, PositionMatrix, Scale, OO_Scale.
class CPhysicsMeshObject : public CBaseClass
{
public:
    virtual ~CPhysicsMeshObject(); // 0x00A127D0

    // Ego_r ?GetAllHelperPointNames@CPhysicsMeshObject@@QBEXAAV?$vector@VCCharString@@...@Z, retail 0x00A11820.
    void GetAllHelperPointNames(_STL::vector<CCharString>& names) const;
    // Ego_r ?GetNumberOfNavigationLineLevels@CPhysicsMeshObject@@QBEJXZ, retail 0x00A127C0.
    long GetNumberOfNavigationLineLevels() const;
    // Ego_r ?IsSpherePartContainedInBoundingBox@CPhysicsMeshObject@@QBE_NABVC3DVector@@M@Z, retail 0x00A11F20.
    bool IsSpherePartContainedInBoundingBox(const C3DVector& centre, float radius) const;
    // Ego_r ?IsPointContainedInBoundingBox@CPhysicsMeshObject@@QBE_NABVC3DVector@@@Z, retail 0x00A12890.
    bool IsPointContainedInBoundingBox(const C3DVector& point) const;

    CIVCountedPointer<const CPhysicsMesh> Mesh; // +0x04
    CMatrix3x4 PositionMatrix;                  // +0x08
    float Scale;                                // +0x38
    float OO_Scale;                             // +0x3C
};

FABLE_STATIC_ASSERT(offsetof(CPhysicsMeshObject, Mesh) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CPhysicsMeshObject, PositionMatrix) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CPhysicsMeshObject, Scale) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CPhysicsMeshObject, OO_Scale) == 0x3C);
FABLE_STATIC_ASSERT(sizeof(CPhysicsMeshObject) == 0x40);
