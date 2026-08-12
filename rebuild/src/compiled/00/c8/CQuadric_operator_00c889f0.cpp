#include <xmmintrin.h>

struct CQuadric {
    __m128 v;
    CQuadric& operator+=(const CQuadric& o);
};

CQuadric& CQuadric::operator+=(const CQuadric& o)
{
    v = _mm_add_ps(v, o.v);
    return *this;
}