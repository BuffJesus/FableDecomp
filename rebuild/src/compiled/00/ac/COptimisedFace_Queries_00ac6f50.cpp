// QuadricOptimiserInternals::COptimisedFace index queries (retail 0x00AC6F50..0x00AC7030, Ego_r 0x00C5F810..0x00C5F8F0):
// the quadric optimiser's edge contraction remaps a collapsed vertex in each face and then drops faces that became
// degenerate; IsIdentical compares faces by their unique (position-welded) vertex indices, in any order.
#include "fable_quadric_optimiser.h"

namespace QuadricOptimiserInternals
{
void COptimisedFace::RemapIndex(unsigned long oldIndex, unsigned long newIndex)
{
    if (VertexIndices[0] == oldIndex) VertexIndices[0] = newIndex;
    if (VertexIndices[1] == oldIndex) VertexIndices[1] = newIndex;
    if (VertexIndices[2] == oldIndex) VertexIndices[2] = newIndex;
}

bool COptimisedFace::HasVertex(unsigned long index) const
{
    if (VertexIndices[0] == index || VertexIndices[1] == index || VertexIndices[2] == index)
        return true;
    return false;
}

bool COptimisedFace::HasUniqueVertex(unsigned long index) const
{
    if (UniqueVertexIndices[0] == index || UniqueVertexIndices[1] == index || UniqueVertexIndices[2] == index)
        return true;
    return false;
}

long COptimisedFace::FindVertex(unsigned long index) const
{
    for (unsigned long i = 0; i < 3; ++i)
    {
        if (VertexIndices[i] == index)
            return i;
    }
    return -1;
}

bool COptimisedFace::IsIdentical(const COptimisedFace& other) const
{
    if (other.HasUniqueVertex(UniqueVertexIndices[0]) && other.HasUniqueVertex(UniqueVertexIndices[1]) &&
        other.HasUniqueVertex(UniqueVertexIndices[2]))
        return true;
    return false;
}

bool COptimisedFace::IsDegenerate() const
{
    unsigned long b = VertexIndices[1], a = VertexIndices[0];
    if (a == b || b == VertexIndices[2] || VertexIndices[2] == a)
        return true;
    return false;
}
}
