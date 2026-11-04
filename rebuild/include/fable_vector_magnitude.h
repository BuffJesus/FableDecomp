#pragma once
#include "rebuild_abi.h"
#include <math.h>
#include "fable_maths_tables.h"
inline float FABLE_FASTCALL GFSqr(float value) { return value*value; }
inline float FABLE_FASTCALL GFRoot(float value) { return (float)sqrt(value); }
// FableWin ?GFFloatAbs@@YIMM@Z (inline in retail; COptimisedPrimitive::MakeOrthogonal 0x00AC7200): clear the sign bit.
inline float FABLE_FASTCALL GFFloatAbs(float value)
{
    *reinterpret_cast<unsigned long*>(&value) &= 0x7FFFFFFF;
    return value;
}
// FableWin ?GFFastOneOverSqrt@@YIMM@Z (inline in retail; C2DVector::Normalise 0x00A14540 and some 230 other sites
// expand it): 1/sqrt(value) from an exponent estimate and the 128-entry mantissa table GFInitMaths builds
// (FableWin GFastOneOverSqrtTable), refined by one Newton step.
inline float FABLE_FASTCALL GFFastOneOverSqrt(float value)
{
    unsigned long bits = *reinterpret_cast<unsigned long*>(&value);
    unsigned long estimate = ((0x17C - ((bits >> 23) & 0xFF)) << 22 & 0xFF800000) |
                             (g_FableInvSqrtTable_013CE558[(bits >> 17) & 0x7F] << 15);
    float result = *reinterpret_cast<float*>(&estimate);
    return result * (3.0f - value * result * result) * 0.5f;
}
