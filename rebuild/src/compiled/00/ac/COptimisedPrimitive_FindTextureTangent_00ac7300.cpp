// QuadricOptimiserInternals::COptimisedPrimitive::FindTextureTangent (retail 0x00AC7300, Ego_r
// lib_3d_quadric_based_mesh_optimiser): the direction across a triangle in which a per-vertex texture coordinate
// increases, scaled so its length is the coordinate's rate of change. The iso-line through the middle vertex runs to the
// point of equal coordinate on the low-high edge; the tangent is perpendicular to it within the face (iso ^ normal), and
// the scale divides by its projection on the low-high edge, so it always points up the gradient. A triangle whose
// coordinate range is within DELTA_NOTIONAL_ZERO has no tangent.
#include "fable_quadric_optimiser.h"

using namespace QuadricOptimiserInternals;

C3DVector COptimisedPrimitive::FindTextureTangent(const C3DVector& p0, const C3DVector& p1, const C3DVector& p2, float t0,
                                                  float t1, float t2)
{
    C3DVector points[3];
    points[0] = p0;
    points[1] = p1;
    points[2] = p2;
    float coords[3] = { t0, t1, t2 };
    long high = (t0 > t1 && t0 > t2) ? 0 : (t1 > t2 ? 1 : 2);
    long low = (t0 < t1 && t0 < t2) ? 0 : (t1 < t2 ? 1 : 2);
    if (coords[low] + DELTA_NOTIONAL_ZERO >= coords[high])
        return C3DVector(0.0f, 0.0f, 0.0f);

    long middle = 0;
    for (unsigned long i = 0; i < 3; ++i)
    {
        if (i != high && i != low)
        {
            middle = i;
            break;
        }
    }
    float lowCoord = coords[low];
    float range = coords[high] - lowCoord;
    float fraction = (coords[middle] - lowCoord) / range;
    C3DVector a = points[low] * (1.0f - fraction);
    C3DVector b = points[high] * fraction;
    C3DVector iso = b + a - points[middle];
    iso.Normalise();
    C3DVector normal = (p2 - p0) ^ (p1 - p0);
    normal.Normalise();
    C3DVector tangent = iso ^ normal;
    tangent.Normalise();
    return tangent * (range / (points[high] - points[low]).Dot(tangent));
}
