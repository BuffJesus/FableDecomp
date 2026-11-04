#pragma once
// Hand-owned (retail model): the physics-mesh face partition tree. Ego_r.pdb names, members, access and virtual order;
// retail RTTI vtables:
//   CIPhysicsFacesSegmentBase  0x0129DE70 (4 bytes: vptr; slots 0-2 and 4 pure, slot 3 ??_G 0x00A85F90)
//   CPhysicsFacesSegmentBranch 0x0129DE88 (0x1C: ChildDividingPlane +0x04, PositiveChild +0x14, NegativeChild +0x18)
//   CPhysicsFacesSegmentLeaf   0x0129DEA4 (0x10 in retail: TriangleListIndex +0x04, a 12-byte STLport CArray; Ego_r 0x14)
// Slot order: HasChildren, Sizeof, GetNoFaces, the deleting destructor, GetPossibleCollisionSet. Ego_r also has
// protected statics (LastSearchIndex, LastSearchIdentifier) and the builders (Create, Calculate,
// CalculateDividingPlane) - not modelled yet. The engine/CPhysicsFacesSegment*.h generated views are shims of this.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_carray.h"
#include "fable_bounding_volumes.h"

class CIPhysicsFacesSegmentBase
{
public:
    virtual bool HasChildren() const = 0;
    virtual long Sizeof() const = 0;
    virtual long GetNoFaces() const = 0;
    // Inline: the base's destructor only restores its own vptr (??_G 0x00A85F90).
    virtual ~CIPhysicsFacesSegmentBase()
    {
    }

protected:
    virtual void GetPossibleCollisionSet(float radius, const C3DVector& start, const C3DVector& end,
                                         _STL::vector<long>& faces) const = 0;
};

class CPhysicsFacesSegmentBranch : public CIPhysicsFacesSegmentBase
{
protected:
    virtual bool HasChildren() const;
    virtual long Sizeof() const;
    virtual long GetNoFaces() const;
    virtual void GetPossibleCollisionSet(float radius, const C3DVector& start, const C3DVector& end,
                                         _STL::vector<long>& faces) const;
    virtual ~CPhysicsFacesSegmentBranch();

private:
    friend struct CPhysicsFacesSegmentLayoutCheck;
    CPlane ChildDividingPlane;                       // +0x04
    CIPhysicsFacesSegmentBase* PositiveChild;        // +0x14
    CIPhysicsFacesSegmentBase* NegativeChild;        // +0x18
};

class CPhysicsFacesSegmentLeaf : public CIPhysicsFacesSegmentBase
{
protected:
    virtual bool HasChildren() const;
    virtual long Sizeof() const;
    virtual long GetNoFaces() const;
    virtual void GetPossibleCollisionSet(float radius, const C3DVector& start, const C3DVector& end,
                                         _STL::vector<long>& faces) const;
    virtual ~CPhysicsFacesSegmentLeaf();

private:
    friend struct CPhysicsFacesSegmentLayoutCheck;
    CArray<unsigned short> TriangleListIndex;        // +0x04
};

struct CPhysicsFacesSegmentLayoutCheck
{
    FABLE_STATIC_ASSERT(offsetof(CPhysicsFacesSegmentBranch, ChildDividingPlane) == 0x04);
    FABLE_STATIC_ASSERT(offsetof(CPhysicsFacesSegmentBranch, PositiveChild) == 0x14);
    FABLE_STATIC_ASSERT(offsetof(CPhysicsFacesSegmentBranch, NegativeChild) == 0x18);
    FABLE_STATIC_ASSERT(offsetof(CPhysicsFacesSegmentLeaf, TriangleListIndex) == 0x04);
};
FABLE_STATIC_ASSERT(sizeof(CIPhysicsFacesSegmentBase) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CPhysicsFacesSegmentBranch) == 0x1C);
FABLE_STATIC_ASSERT(sizeof(CPhysicsFacesSegmentLeaf) == 0x10);
