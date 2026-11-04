// C2DVector::Normalise (retail 0x00A14540, Ego_r lib_2d_vector): scale the vector to unit length with the engine's fast
// reciprocal square root.
#include "fable_2d_vector.h"
#include "fable_vector_magnitude.h"

C2DVector& C2DVector::Normalise()
{
    float oneOnMagnitude = GFFastOneOverSqrt(GFSqr(X) + GFSqr(Y));
    X *= oneOnMagnitude;
    Y *= oneOnMagnitude;
    return *this;
}
