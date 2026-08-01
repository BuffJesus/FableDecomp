// Self-contained behavior test for _Copy_backward<CHeroMarriageInfo*,...>.
// Compiles under cl /Od, links against source.obj, runs.
#include <stdio.h>

struct CHeroMarriageInfo
{
    unsigned long m[7];
};

struct _Nonscalar_ptr_iterator_tag {};

extern CHeroMarriageInfo* __fastcall _Copy_backward(
    CHeroMarriageInfo* _First,
    CHeroMarriageInfo* _Last,
    CHeroMarriageInfo* _Dest,
    CHeroMarriageInfo* _Dest2,
    _Nonscalar_ptr_iterator_tag);

int main()
{
    const int N = 5;
    CHeroMarriageInfo src[5];
    CHeroMarriageInfo dst[8];
    int i, j;

    for (i = 0; i < N; ++i)
        for (j = 0; j < 7; ++j)
            src[i].m[j] = (unsigned long)(i * 100 + j);

    for (i = 0; i < 8; ++i)
        for (j = 0; j < 7; ++j)
            dst[i].m[j] = 0xEEEEEEEEUL;

    // copy_backward [src, src+N) into [..., dst+8): fills dst[3..7]
    _Nonscalar_ptr_iterator_tag tag;
    CHeroMarriageInfo* ret = _Copy_backward(src, src + N, dst + 8, dst + 8, tag);

    int ok = 1;

    // return value = dst + 8 - N = dst + 3
    if (ret != dst + 3) ok = 0;

    // dst[3..7] should equal src[0..4]
    for (i = 0; i < N && ok; ++i)
        for (j = 0; j < 7; ++j)
            if (dst[3 + i].m[j] != src[i].m[j]) { ok = 0; break; }

    // dst[0..2] untouched
    for (i = 0; i < 3 && ok; ++i)
        for (j = 0; j < 7; ++j)
            if (dst[i].m[j] != 0xEEEEEEEEUL) { ok = 0; break; }

    // empty-range no-op
    if (ok)
    {
        CHeroMarriageInfo* r2 = _Copy_backward(src, src, dst + 3, dst + 3, tag);
        if (r2 != dst + 3) ok = 0;
    }

    if (ok)
        printf("COPY_BACKWARD_CHEROMARRIAGEINFO_TEST PASS\n");
    else
        printf("COPY_BACKWARD_CHEROMARRIAGEINFO_TEST FAIL\n");
    return ok ? 0 : 1;
}