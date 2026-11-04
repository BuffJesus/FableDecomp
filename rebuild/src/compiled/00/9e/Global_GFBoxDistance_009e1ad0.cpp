// lib_global_tools.cpp, retail 0x009E1AD0 / 0x009E1B00 / 0x009E1B30 / 0x009E1B80
// (Ego_r GFGet2DBoxDistance and GFGet3DBoxDistance, float and integer forms):
// the largest per-axis distance (Chebyshev). The 3D forms take Z first.
#include <math.h>
#include <stdlib.h>
#include "fable_2d_vector.h"
#include "fable_display_geometry.h"

// Retail 009E1AD0 (?GFGet2DBoxDistance@@YIMABVC2DVector@@0@Z).
float FABLE_FASTCALL GFGet2DBoxDistance(const C2DVector& first, const C2DVector& second)
{
    float dx = (float)fabs(first.X - second.X);
    float dy = (float)fabs(first.Y - second.Y);
    return dx > dy ? dx : dy;
}

// Retail 009E1B00 (?GFGet2DBoxDistance@@YIJABVC2DCoordI@@0@Z).
long FABLE_FASTCALL GFGet2DBoxDistance(const C2DCoordI& first, const C2DCoordI& second)
{
    long dx = labs(first.X - second.X);
    long dy = labs(first.Y - second.Y);
    return dx > dy ? dx : dy;
}

// Retail 009E1B30 (?GFGet3DBoxDistance@@YIMABVC3DVector@@0@Z).
float FABLE_FASTCALL GFGet3DBoxDistance(const C3DVector& first, const C3DVector& second)
{
    float dz = (float)fabs(first.Z - second.Z);
    float dx = (float)fabs(first.X - second.X);
    float dy = (float)fabs(first.Y - second.Y);
    float d = dx > dy ? dx : dy;
    return d > dz ? d : dz;
}

// Retail 009E1B80 (?GFGet3DBoxDistance@@YIJABVC3DCoordI@@0@Z).
long FABLE_FASTCALL GFGet3DBoxDistance(const C3DCoordI& first, const C3DCoordI& second)
{
    long dz = labs(first.Z - second.Z);
    long dx = labs(first.X - second.X);
    long dy = labs(first.Y - second.Y);
    long d = dx > dy ? dx : dy;
    return d > dz ? d : dz;
}
