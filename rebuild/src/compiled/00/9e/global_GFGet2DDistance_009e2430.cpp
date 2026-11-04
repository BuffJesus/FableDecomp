// lib_global_tools.cpp, retail 0x009E2430 / 0x009E2460 / 0x009E2480: the three
// GFGet2DDistance overloads (Ego_r signatures). The integer-point form squares
// in integers; the others work in floats. All end in an inline fsqrt. This
// replaces an earlier landing that took pointers to a local struct (a different
// mangled name).
#include <math.h>
#include "fable_2d_vector.h"
#include "fable_display_geometry.h"

// Retail 009E2430 (?GFGet2DDistance@@YIMABVC2DCoordI@@0@Z).
float FABLE_FASTCALL GFGet2DDistance(const C2DCoordI& first, const C2DCoordI& second)
{
    long dx = first.X - second.X;
    long dy = first.Y - second.Y;
    return sqrtf((float)(dy * dy + dx * dx));
}

// Retail 009E2460 (?GFGet2DDistance@@YIMABVC2DVector@@0@Z).
float FABLE_FASTCALL GFGet2DDistance(const C2DVector& first, const C2DVector& second)
{
    float dx = first.X - second.X;
    float dy = first.Y - second.Y;
    return sqrtf(dy * dy + dx * dx);
}

// Retail 009E2480 (?GFGet2DDistance@@YIMJJJJ@Z).
float FABLE_FASTCALL GFGet2DDistance(long x1, long y1, long x2, long y2)
{
    float dx = (float)(x2 - x1);
    float dy = (float)(y2 - y1);
    return sqrtf(dy * dy + dx * dx);
}
