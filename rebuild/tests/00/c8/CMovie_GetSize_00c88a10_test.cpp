#include <xmmintrin.h>
#include <stdio.h>

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

int main()
{
    __declspec(align(16)) CMovie m;
    m.v.v = _mm_setr_ps(1.0f, 2.0f, 3.0f, 4.0f);
    __declspec(align(16)) V4 o;
    o.v = _mm_setr_ps(5.0f, 6.0f, 7.0f, 8.0f);

    V4& r = m.GetSize(o);
    float out[4];
    _mm_storeu_ps(out, r.v);

    if (out[0] == 5.0f && out[1] == 12.0f && out[2] == 21.0f && out[3] == 32.0f
        && &r == &m.v) {
        printf("GETSIZE_OK\n");
        return 0;
    }
    printf("GETSIZE_FAIL %f %f %f %f\n", out[0], out[1], out[2], out[3]);
    return 1;
}