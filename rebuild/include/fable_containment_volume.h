#pragma once
// Hand-owned (moved out of the generated engine/CContainmentVolume.h so gen_class_headers leaves it alone):
// Ego_r.pdb CContainmentVolume (0x10) with its real virtual destructor; retail vtable 0x0129DC94.
#include <stddef.h>
#include "rebuild_abi.h"

class CMovableResourceMemoryPool;
class CPlane;
class C3DVector;

#pragma pack(push, 1)
class CContainmentVolume {
public:
    // Retail vtable 0x0129DC94 (RTTI .?AVCContainmentVolume@@): slot 0 is the
    // vector deleting destructor 0x00A758E0 (constructor 0x00A752D0).
    // Retail RTTI: CContainmentVolume : CMovableResource (slots 1..3 RelocateData
    // 0x00A755A0, 0x009D50F0, Verify 0x009D5100). The base is not modelled while
    // engine/CMovableResource.h is a flat POD view with a __vftable member.
    virtual ~CContainmentVolume();
    // Ego_r CContainmentVolume queries (engine_api: retail 0x00A75340 / 0x00A75430 / 0x00A75460 /
    // 0x00A754A0); C3DMeshVolume 0x00A2E700..0x00A2E730 forwards to them.
    float PeekRoughProportionOfSphereContained(const C3DVector& centre, float radius) const;
    bool IsContainingPoint(const C3DVector& point) const;
    bool IsContainingSphere(const C3DVector& centre, float radius) const;
    bool IsContainingPartOfSphere(const C3DVector& centre, float radius) const;
    CPlane*                     ContainmentPlanes;           // +0x04
    unsigned long               Size;                        // +0x08
    CMovableResourceMemoryPool* MemoryPool;                  // +0x0c
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CContainmentVolume) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CContainmentVolume, ContainmentPlanes) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CContainmentVolume, Size) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CContainmentVolume, MemoryPool) == 0xc);
