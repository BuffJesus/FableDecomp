// CMovie::GetSize @ 0x00c88a10
// packed SSE: xmm0 = *arg, xmm1 = *this, xmm1 *= xmm0, *this = xmm1, return this.
#include <xmmintrin.h>

struct V4 {
    __m128 v;
};

struct CMovie {
    V4 v;
    V4& GetSize(const V4& o);
};

V4& CMovie::GetSize(const V4& o)
{
    v.v = _mm_mul_ps(v.v, o.v);
    return v;
}