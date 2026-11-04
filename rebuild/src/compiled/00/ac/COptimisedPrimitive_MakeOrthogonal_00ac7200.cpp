// QuadricOptimiserInternals::COptimisedPrimitive::MakeOrthogonal (retail 0x00AC7200, Ego_r
// lib_3d_quadric_based_mesh_optimiser): remove the axis component from a vector and give the remainder the vector's
// original length. A (nearly) zero vector comes back unchanged; a remainder too short to rescale comes back as is.
#include "fable_quadric_optimiser.h"

using namespace QuadricOptimiserInternals;

C3DVector COptimisedPrimitive::MakeOrthogonal(const C3DVector& vector, const C3DVector& axis) const
{
    C3DVector result = vector;
    float length = vector.GetAccurateMagnitude();
    if (GFFloatAbs(length) > DELTA_NOTIONAL_ZERO)
    {
        result -= axis * vector.Dot(axis);
        float squared = result.GetSquaredMagnitude();
        if (squared > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
            result *= length / GFRoot(squared);
    }
    return result;
}
