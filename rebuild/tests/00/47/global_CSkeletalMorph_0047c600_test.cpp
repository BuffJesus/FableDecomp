#include <cstdio>

// 24-byte element split into two sub-object members with out-of-line op=.
struct MemberA { unsigned long _p; MemberA& operator=(const MemberA& r); };
struct MemberB { unsigned long _p0, _p1, _p2; MemberB& operator=(const MemberB& r); };
struct CSkeletalMorph { unsigned long _a; MemberA _b; unsigned long _c; MemberB _d; };
struct _Nonscalar_ptr_iterator_tag {};

// Out-of-line member operator= definitions (the masked callees). Real field copies.
MemberA& MemberA::operator=(const MemberA& r) { _p = r._p; return *this; }
MemberB& MemberB::operator=(const MemberB& r) { _p0 = r._p0; _p1 = r._p1; _p2 = r._p2; return *this; }

CSkeletalMorph* __fastcall Copy_impl_CSkeletalMorph_0047c600(
    CSkeletalMorph* _First,
    CSkeletalMorph* _Last,
    CSkeletalMorph* _Dest,
    CSkeletalMorph* _Dest2,
    _Nonscalar_ptr_iterator_tag)
{
    (void)_Dest2;
    for (int _N = (int)(_Last - _First); 0 < _N; --_N) {
        *_Dest = *_First;
        ++_First;
        ++_Dest;
    }
    return (_Dest);
}

static void fill(CSkeletalMorph& e, unsigned long base) {
    e._a = base + 0; e._b._p = base + 1; e._c = base + 2;
    e._d._p0 = base + 3; e._d._p1 = base + 4; e._d._p2 = base + 5;
}
static bool eq(const CSkeletalMorph& x, const CSkeletalMorph& y) {
    return x._a == y._a && x._b._p == y._b._p && x._c == y._c &&
           x._d._p0 == y._d._p0 && x._d._p1 == y._d._p1 && x._d._p2 == y._d._p2;
}

int main() {
    const int N = 3;
    CSkeletalMorph src[N], dst[N];
    int i;
    for (i = 0; i < N; ++i) { fill(src[i], 1000u * (i + 1)); fill(dst[i], 0xFFFF0000u); }

    _Nonscalar_ptr_iterator_tag tag;
    CSkeletalMorph* end = Copy_impl_CSkeletalMorph_0047c600(&src[0], &src[N], &dst[0], (CSkeletalMorph*)0, tag);
    if (end != &dst[N]) { std::printf("FAIL end ptr\n"); return 1; }
    for (i = 0; i < N; ++i)
        if (!eq(dst[i], src[i])) { std::printf("FAIL elem %d\n", i); return 1; }

    // Empty range returns _Dest unchanged.
    CSkeletalMorph* e2 = Copy_impl_CSkeletalMorph_0047c600(&src[0], &src[0], &dst[0], (CSkeletalMorph*)0, tag);
    if (e2 != &dst[0]) { std::printf("FAIL empty ptr\n"); return 1; }

    std::printf("COPY_IMPL_CSKELETALMORPH_0047C600_TEST PASS\n");
    return 0;
}