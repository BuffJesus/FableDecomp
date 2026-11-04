// QuadricOptimiserInternals::COptimisedPrimitive::GetFaceNormal (retail 0x00AC7CB0, Ego_r lib_3d_quadric_based_mesh_
// optimiser): the cross product of the face's two edges from its first vertex, normalised unless it is (nearly) zero.
#include "fable_quadric_optimiser.h"

using namespace QuadricOptimiserInternals;

C3DVector COptimisedPrimitive::GetFaceNormal(const COptimisedFace& face) const
{
    const C3DVector& p0 = Vertices[face.VertexIndices[0]].Position;
    const C3DVector& p1 = Vertices[face.VertexIndices[1]].Position;
    const C3DVector& p2 = Vertices[face.VertexIndices[2]].Position;
    C3DVector edge1 = p1 - p0;
    C3DVector edge2 = p2 - p0;
    C3DVector normal = edge2 ^ edge1;
    if (normal.IsNonZero())
        normal.Normalise();
    return normal;
}
