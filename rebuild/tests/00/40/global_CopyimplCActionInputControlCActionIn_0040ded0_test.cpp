#include <cstdio>

struct CActionInputControl { int m[7]; };

CActionInputControl* __fastcall Copy_impl(CActionInputControl* _First, CActionInputControl* _Last, CActionInputControl* _Dest, int _pad)
{
    for (; _First != _Last; ++_First, ++_Dest)
        if (_Dest)
            *_Dest = *_First;
    return _Dest;
}

int main()
{
    CActionInputControl src[3];
    CActionInputControl dst[3];
    int i, j;
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 7; ++j) { src[i].m[j] = i * 100 + j; dst[i].m[j] = -1; }
    CActionInputControl* end = Copy_impl(&src[0], &src[3], &dst[0], 0);
    if (end != &dst[3]) { std::printf("FAIL end ptr\n"); return 1; }
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 7; ++j)
            if (dst[i].m[j] != i * 100 + j) { std::printf("FAIL data %d %d\n", i, j); return 1; }
    std::printf("_global_0040ded0_TEST PASS\n");
    return 0;
}