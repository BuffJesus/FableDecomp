// CCamera::GetAngleXY @ 00A0C380; original orientation in turns with unit wrap.
#include "fable_camera.h"
#include <math.h>
float CCamera::GetAngleXY() const
{
    float angle = static_cast<float>(atan2(RHSet.Forward.X, RHSet.Forward.Y)) * 0.15915494309189535f;
    if (angle < 0.0f || angle >= 1.0f)
    {
        angle = static_cast<float>(fmod(static_cast<double>(angle), 1.0));
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}
