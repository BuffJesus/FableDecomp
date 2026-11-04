#pragma once
#include "fable_2d_vector.h"
#include <math.h>

__forceinline float C3DVector::GetAngleXY() const
{
    float angle = static_cast<float>(atan2(X, Y)) * 0.15915494309189535f;
    if (angle < 0.0f || angle >= 1.0f)
    {
        angle = static_cast<float>(fmod(static_cast<double>(angle), 1.0));
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

__forceinline float C3DVector::GetAngleYZ() const
{
    float angle = static_cast<float>(atan2(Z, sqrt(X * X + Y * Y))) * 0.15915494309189535f;
    if (angle < 0.0f || angle >= 1.0f)
    {
        angle = static_cast<float>(fmod(static_cast<double>(angle), 1.0));
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

// FableWin C3DVector::InitialiseFromAngles and GF trig helpers.
inline const C3DVector& C3DVector::InitialiseFromAngles(float angle_xy,float angle_yz,float distance)
{
    double yz = angle_yz * 6.2831854820251465;
    float cos_yz = static_cast<float>(cos(yz));
    double xy = angle_xy * 6.2831854820251465;
    X = static_cast<float>(sin(xy) * cos_yz * distance);
    Y = static_cast<float>(cos(xy) * cos_yz * distance);
    Z = static_cast<float>(sin(yz) * distance);
    return *this;
}
inline float FABLE_FASTCALL GFDotProduct(const C3DVector& a,const C3DVector& b)
{
    return a.X*b.X + a.Y*b.Y + a.Z*b.Z;
}
inline float FABLE_FASTCALL GFArcCos(double value)
{
    return static_cast<float>(acos(value)) * 0.15915494309189535f;
}
