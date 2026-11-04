// QuadricOptimiserInternals::COptimisedPrimitive::FindFaceTextureTangents (retail 0x00AC7E10, Ego_r
// lib_3d_quadric_based_mesh_optimiser): the face's U tangent from its vertices' texture U coordinates and its V tangent
// from their V coordinates.
#include "fable_quadric_optimiser.h"

using namespace QuadricOptimiserInternals;

void COptimisedPrimitive::FindFaceTextureTangents(COptimisedFace& face)
{
    const COptimisedVertex& v0 = Vertices[face.VertexIndices[0]];
    const COptimisedVertex& v1 = Vertices[face.VertexIndices[1]];
    const COptimisedVertex& v2 = Vertices[face.VertexIndices[2]];
    face.UTangent = FindTextureTangent(v0.Position, v1.Position, v2.Position,
                                       v0.TextureCoord.X, v1.TextureCoord.X, v2.TextureCoord.X);
    face.VTangent = FindTextureTangent(v0.Position, v1.Position, v2.Position,
                                       v0.TextureCoord.Y, v1.TextureCoord.Y, v2.TextureCoord.Y);
}
