#pragma once
// Hand-owned mesh statistics family (Ego_r.pdb MeshStats.cpp): C3DHelperPoint, C3DDummyObject,
// CPreAllocVectorMap, C3DMeshVolume and a partial C3DMeshStats. Names and members are Ego_r's
// (ghidra_out/struct_layouts_egor.tsv); no generated engine/ header exists for these classes.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_string.h"
#include "fable_counted_pointer.h"
#include "fable_vector_map.h"
#include "fable_movable_resource.h"
#include "fable_containment_volume.h"
#include "fable_2d_vector.h"
#include "fable_bounding_volumes.h"
#include "engine/CMatrix3x4.h"

class CMovableResourceMemoryPool;

// Ego_r C3DHelperPoint (0x10): C3DVector Position, long BoneIndex.
class C3DHelperPoint
{
public:
    C3DVector Position;                                      // +0x00
    long BoneIndex;                                          // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(C3DHelperPoint) == 0x10);
FABLE_STATIC_ASSERT(offsetof(C3DHelperPoint, BoneIndex) == 0x0c);

// Ego_r C3DDummyObject (0x34): CMatrix3x4 Transform, long BoneIndex.
class C3DDummyObject
{
public:
    CMatrix3x4 Transform;                                    // +0x00
    long BoneIndex;                                          // +0x30
};
FABLE_STATIC_ASSERT(sizeof(C3DDummyObject) == 0x34);
FABLE_STATIC_ASSERT(offsetof(C3DDummyObject, BoneIndex) == 0x30);

// Ego_r CPreAllocVectorMap<K,V,C> (0x10): a sorted pair array in caller-provided storage.
// Retail LowerBound/UpperBound are out of line and read Elements +4 and Size +8
// (<unsigned long, C3DHelperPoint>: 0x00A316B0 / 0x00A31700, 0x14-byte pairs;
// <unsigned long, C3DDummyObject>: 0x00A31790 / 0x00A317E0, 0x38-byte pairs).
template <class K, class V, class C>
class CPreAllocVectorMap
{
public:
    const _STL::pair<K, V>* LowerBound(const K& key) const;
    const _STL::pair<K, V>* UpperBound(const K& key) const;

    C Compare;                                               // +0x00 (empty)
    _STL::pair<K, V>* Elements;                              // +0x04
    unsigned long Size;                                      // +0x08
    bool Dirty;                                              // +0x0c
};

// Ego_r C3DMeshVolume (0x0c, no vtable): CCharString Name, CCountedPointer<CContainmentVolume>
// ConvexVolume (Data +0x04, Info +0x08). Ego_r MeshStats.cpp order b8de50..b8dea0 = retail
// 0x00A2E700..0x00A2E730; each query forwards to the convex volume.
class C3DMeshVolume
{
public:
    float PeekRoughProportionOfSphereContained(const C3DVector& centre, float radius) const;
    bool IsContainingPoint(const C3DVector& point) const;
    bool IsContainingSphere(const C3DVector& centre, float radius) const;
    bool IsContainingPartOfSphere(const C3DVector& centre, float radius) const;

    CCharString Name;                                        // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;        // +0x04
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshVolume) == 0x0c);
FABLE_STATIC_ASSERT(offsetof(C3DMeshVolume, ConvexVolume) == 0x04);

// Partial view of Ego_r C3DMeshStats (0x84 in Ego_r) : CMovableResource. Retail vtable 0x0129CF04
// (RTTI .?AVC3DMeshStats@@ : CMovableResource): [0] ??_G 0x00A353F0, [1] RelocateData 0x00A2E820
// (Ego_r ?RelocateData@C3DMeshStats@@UAEXJPAX0J@Z), [2]/[3] CMovableResource's 0x009D50F0 /
// 0x009D5100. Members after DummyObjects (MeshVolumes, MeshGenerators, PackedNames, ...) are not
// recovered.
class C3DMeshStats : public CMovableResource
{
public:
    virtual ~C3DMeshStats();
    virtual void RelocateData(long handle, void* oldAddress, void* newAddress, long size);
    // Ego_r MeshStats.cpp: FindHelperPointsBegin/End, FindDummyObjectsBegin/End
    // (retail 0x00A2E900 / 0x00A2E910 / 0x00A2E960 / 0x00A2E970).
    const _STL::pair<unsigned long, C3DHelperPoint>* FindHelperPointsBegin(unsigned long name) const;
    const _STL::pair<unsigned long, C3DHelperPoint>* FindHelperPointsEnd(unsigned long name) const;
    const _STL::pair<unsigned long, C3DDummyObject>* FindDummyObjectsBegin(unsigned long name) const;
    const _STL::pair<unsigned long, C3DDummyObject>* FindDummyObjectsEnd(unsigned long name) const;

    CMovableResourceMemoryPool* MemoryPool;                  // +0x04
    long StatsMemoryHandle;                                  // +0x08
    CCharString Name;                                        // +0x0c
    C3DBoundingSphere BoundingSphere;                        // +0x10
    C3DBoundingBox BoundingBox;                              // +0x20
    CPreAllocVectorMap<unsigned long, C3DHelperPoint,
        CKeyPairCompareLess<unsigned long, C3DHelperPoint> > HelperPoints; // +0x38
    CPreAllocVectorMap<unsigned long, C3DDummyObject,
        CKeyPairCompareLess<unsigned long, C3DDummyObject> > DummyObjects; // +0x48
};
FABLE_STATIC_ASSERT(offsetof(C3DMeshStats, MemoryPool) == 0x04);
FABLE_STATIC_ASSERT(offsetof(C3DMeshStats, Name) == 0x0c);
FABLE_STATIC_ASSERT(offsetof(C3DMeshStats, BoundingSphere) == 0x10);
FABLE_STATIC_ASSERT(offsetof(C3DMeshStats, BoundingBox) == 0x20);
FABLE_STATIC_ASSERT(offsetof(C3DMeshStats, HelperPoints) == 0x38);
FABLE_STATIC_ASSERT(offsetof(C3DMeshStats, DummyObjects) == 0x48);
