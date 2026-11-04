// The face partition tree's small members (retail 0x00A85FB0..0x00A86030, 0x00A860F0, 0x00A86100, 0x00A86F10,
// 0x00A87110, 0x00A87140): a branch accounts for and owns its two children; a leaf lists triangle indices.
#include "fable_physics_faces_segment.h"

// 0x00A86F10
bool CPhysicsFacesSegmentBranch::HasChildren() const
{
    return true;
}

// 0x00A85FB0
long CPhysicsFacesSegmentBranch::Sizeof() const
{
    return PositiveChild->Sizeof() + NegativeChild->Sizeof() + sizeof(CPhysicsFacesSegmentBranch);
}

// 0x00A85FD0
long CPhysicsFacesSegmentBranch::GetNoFaces() const
{
    return PositiveChild->GetNoFaces() + NegativeChild->GetNoFaces();
}

// 0x00A85FF0
CPhysicsFacesSegmentBranch::~CPhysicsFacesSegmentBranch()
{
    delete PositiveChild;
    PositiveChild = 0;
    delete NegativeChild;
    NegativeChild = 0;
}

// 0x00A87110
bool CPhysicsFacesSegmentLeaf::HasChildren() const
{
    return false;
}

// 0x00A86100
long CPhysicsFacesSegmentLeaf::Sizeof() const
{
    return TriangleListIndex.Sizeof();
}

// 0x00A860F0
long CPhysicsFacesSegmentLeaf::GetNoFaces() const
{
    return TriangleListIndex.size();
}

// 0x00A87140
CPhysicsFacesSegmentLeaf::~CPhysicsFacesSegmentLeaf()
{
}
