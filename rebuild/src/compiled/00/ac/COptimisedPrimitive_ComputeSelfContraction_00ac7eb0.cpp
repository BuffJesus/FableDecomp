// QuadricOptimiserInternals::COptimisedPrimitive::ComputeSelfContraction (retail 0x00AC7EB0, Ego_r
// lib_3d_quadric_based_mesh_optimiser): an unsplit pair (two vertices at one position that differ in texture coordinate
// or normal) collapses onto its first vertex. Its cost weighs the squared texture-coordinate and normal distances, with
// the high penalty when the normals face apart.
#include "fable_quadric_optimiser.h"

using namespace QuadricOptimiserInternals;

void COptimisedPrimitive::ComputeSelfContraction(CVertexPairContraction& contraction)
{
    const CSubContraction& pair = contraction.SubContractions[0];
    const COptimisedVertex& first = Vertices[pair.Index1];
    const COptimisedVertex& second = Vertices[pair.Index2];
    float textureDistance = (second.TextureCoord - first.TextureCoord).GetAccurateMagnitude();
    float normalDistance = (second.Normal - first.Normal).GetAccurateMagnitude();
    float cost = textureDistance * textureDistance * Parameters.UnsplitTextureWeight;
    cost += normalDistance * normalDistance * Parameters.UnsplitNormalWeight;
    if (first.Normal.Dot(second.Normal) < 0.0f)
        cost += Parameters.HighPenalty;
    contraction.Cost = cost;
    contraction.ResultingPosition = first.Position;
    contraction.Weight2 = 0.5f;
}
