#include "fable_2d_vector.h"
#include "fable_vector_magnitude.h"
C3DVector& C3DVector::AccurateNormalise()
{
    float one_on_magnitude = 1.0f/GFRoot(GFSqr(X)+GFSqr(Y)+GFSqr(Z));
    X *= one_on_magnitude;
    Y *= one_on_magnitude;
    Z *= one_on_magnitude;
    return *this;
}
