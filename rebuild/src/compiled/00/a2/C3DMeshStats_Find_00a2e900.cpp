// C3DMeshStats helper-point / dummy-object range lookups (Ego_r MeshStats.cpp order
// FindHelperPointsBegin, FindHelperPointsEnd, ..., FindDummyObjectsBegin, FindDummyObjectsEnd):
// retail 0x00A2E900 / 0x00A2E910 (HelperPoints +0x38, LowerBound 0x00A316B0 / UpperBound
// 0x00A31700) and 0x00A2E960 / 0x00A2E970 (DummyObjects +0x48, LowerBound 0x00A31790 /
// UpperBound 0x00A317E0). The retail caller 0x00A36160 takes End - Begin. The manifest label
// "CTCInventoryAbilities::AddLearnedExpression" is a propagated name.
#include "fable_mesh_stats.h"

const _STL::pair<unsigned long, C3DHelperPoint>* C3DMeshStats::FindHelperPointsBegin(unsigned long name) const
{
    return HelperPoints.LowerBound(name);
}

const _STL::pair<unsigned long, C3DHelperPoint>* C3DMeshStats::FindHelperPointsEnd(unsigned long name) const
{
    return HelperPoints.UpperBound(name);
}

const _STL::pair<unsigned long, C3DDummyObject>* C3DMeshStats::FindDummyObjectsBegin(unsigned long name) const
{
    return DummyObjects.LowerBound(name);
}

const _STL::pair<unsigned long, C3DDummyObject>* C3DMeshStats::FindDummyObjectsEnd(unsigned long name) const
{
    return DummyObjects.UpperBound(name);
}
