#pragma once
// Hand-owned (Ego_r.pdb names): NVectorMath strided vector helpers.
class C4DVector;

namespace NVectorMath
{
    // Ego_r ?GFInterpolateVectors@NVectorMath@@YAXPAVC4DVector@@KPBV2@K1KPBMKK@Z (retail 0x00A5BEF0, not yet
    // reconstructed): interpolate count vectors, every array walked with its own byte stride (body not yet read).
    void GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* from, unsigned long fromStride,
                              const C4DVector* to, unsigned long toStride, const float* t, unsigned long tStride,
                              unsigned long count);
    // Ego_r ?GFInterpolateVectors@NVectorMath@@YAXPAVC4DVector@@KPBV2@K1KKM@Z (retail 0x00A5BBA0, not yet reconstructed):
    // out = interpolation of from and to by one factor t.
    void GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* from, unsigned long fromStride,
                              const C4DVector* to, unsigned long toStride, unsigned long count, float t);
    // Ego_r ?GFInterpolateVectors@NVectorMath@@YAXPAVC4DVector@@KPBV2@KKM@Z (retail 0x00A5B870, not yet reconstructed):
    // move out towards to by t.
    void GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* to, unsigned long toStride,
                              unsigned long count, float t);
    // Ego_r ?GFClear4DVectors@NVectorMath@@YAXPAVC4DVector@@KABV2@@Z (retail 0x00A5C300, not yet reconstructed): set
    // count vectors to value.
    void GFClear4DVectors(C4DVector* vectors, unsigned long count, const C4DVector& value);
}
