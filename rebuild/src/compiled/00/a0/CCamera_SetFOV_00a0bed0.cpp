#include "fable_camera.h"
void CCamera::SetFOV(float h_angle,float v_angle)
{
    HorizontalFOV = h_angle;
    VerticalFOV = v_angle;
    FOVFlags = 1;
    if (v_angle > 0.0f)
        FOVFlags |= 2;
}
