// CCamera::GetAngleXZ @ 00A0C450; donor locals and original math helper names.
#include "fable_camera.h"
#include "fable_vector_angles.h"
float CCamera::GetAngleXZ() const
{
    C3DVector no_xz_up;
    float up_anglexy = RHSet.GetForward().GetAngleXY() + 0.5f;
    float up_angleyz = 0.25f - RHSet.GetForward().GetAngleYZ();
    no_xz_up.InitialiseFromAngles(up_anglexy, up_angleyz, 1.0f);
    float cos_xz = GFDotProduct(no_xz_up, RHSet.GetUp());
    return GFArcCos(cos_xz);
}
