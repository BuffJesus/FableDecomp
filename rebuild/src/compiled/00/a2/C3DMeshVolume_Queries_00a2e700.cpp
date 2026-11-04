// C3DMeshVolume containment queries, retail 0x00A2E700..0x00A2E730 (Ego_r MeshStats.cpp order
// b8de50..b8dea0): each forwards to its convex volume (CCountedPointer<CContainmentVolume>
// ConvexVolume, Data at +0x04), which VC7.1 emits as `mov ecx,[ecx+4]; jmp CContainmentVolume::...`
// (0x00A75340, 0x00A75430, 0x00A75460, 0x00A754A0). Callers: the C3DMeshStats volume walks
// 0x00A6CA80 / 0x00A6CA20 and 0x00A73D10. The manifest "_Cons_val<...EHeroMorphType...>" labels
// are propagated names.
#include "fable_mesh_stats.h"

float C3DMeshVolume::PeekRoughProportionOfSphereContained(const C3DVector& centre, float radius) const
{
    return ConvexVolume->PeekRoughProportionOfSphereContained(centre, radius);
}

bool C3DMeshVolume::IsContainingPoint(const C3DVector& point) const
{
    return ConvexVolume->IsContainingPoint(point);
}

bool C3DMeshVolume::IsContainingSphere(const C3DVector& centre, float radius) const
{
    return ConvexVolume->IsContainingSphere(centre, radius);
}

bool C3DMeshVolume::IsContainingPartOfSphere(const C3DVector& centre, float radius) const
{
    return ConvexVolume->IsContainingPartOfSphere(centre, radius);
}
