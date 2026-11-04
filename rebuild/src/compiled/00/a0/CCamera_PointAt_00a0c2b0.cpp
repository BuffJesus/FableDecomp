#include "fable_camera.h"
#include "fable_vector_angles.h"
void CCamera::PointAt(const C3DVector& target_position)
{
    if (ViewVecZLocked)
        RHSet.SetForward(C3DVector(target_position.X-WorldPosition.X,target_position.Y-WorldPosition.Y,0.0f));
    else
        RHSet.SetForward(target_position-WorldPosition);
    RHSet.Normalise();
    float factor = GFDotProduct(C3DVector(0.0f,0.0f,1.0f),RHSet.GetForward());
    C3DVector difference = RHSet.GetForward()*factor;
    RHSet.SetUp(C3DVector(0.0f,0.0f,1.0f)-difference);
    RHSet.Normalise();
}
