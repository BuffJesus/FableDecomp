#include <xmmintrin.h>
#include <stdio.h>

struct CQuadric {
    __m128 v;
    CQuadric& operator+=(const CQuadric& o);
};

CQuadric& CQuadric::operator+=(const CQuadric& o)
{
    v = _mm_add_ps(v, o.v);
    return *this;
}

int main()
{
    __declspec(align(16)) float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    __declspec(align(16)) float b[4] = {10.0f, 20.0f, 30.0f, 40.0f};
    CQuadric qa; qa.v = _mm_load_ps(a);
    CQuadric qb; qb.v = _mm_load_ps(b);
    qa += qb;
    __declspec(align(16)) float r[4];
    _mm_store_ps(r, qa.v);
    if (r[0]==11.0f && r[1]==22.0f && r[2]==33.0f && r[3]==44.0f)
        printf("QUADRIC_ADD_OK\n");
    else
        printf("FAIL %f %f %f %f\n", r[0], r[1], r[2], r[3]);
    return (r[0]==11.0f && r[1]==22.0f && r[2]==33.0f && r[3]==44.0f) ? 0 : 1;
}