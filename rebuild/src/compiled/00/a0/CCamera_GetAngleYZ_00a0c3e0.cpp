// CCamera::GetAngleYZ @ 00A0C3E0; original orientation in turns with unit wrap.
#include "fable_camera.h"
#include <math.h>
float CCamera::GetAngleYZ() const
{
    float angle = static_cast<float>(atan2(RHSet.Forward.Z, sqrt(RHSet.Forward.X * RHSet.Forward.X + RHSet.Forward.Y * RHSet.Forward.Y))) * 0.15915494309189535f;
    if (angle < 0.0f || angle >= 1.0f)
    {
        angle = static_cast<float>(fmod(static_cast<double>(angle), 1.0));
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}
