#include <cstdio>

struct WVec { long begin; long end; };
struct CWideString { WVec* vec; };

unsigned long __fastcall Sizeof(CWideString* self);

int main()
{
    CWideString a; a.vec = 0;
    unsigned long r0 = Sizeof(&a);

    // end-begin = 10 => count=5 => 5*2+4 = 14
    WVec v; v.begin = 100; v.end = 110;
    CWideString b; b.vec = &v;
    unsigned long r1 = Sizeof(&b);

    WVec v2; v2.begin = 200; v2.end = 200;
    CWideString c; c.vec = &v2;
    unsigned long r2 = Sizeof(&c);

    if (r0 == 4 && r1 == 14 && r2 == 4)
        printf("0099b2a0_TEST PASS\n");
    else
        printf("FAIL r0=%lu r1=%lu r2=%lu\n", r0, r1, r2);
    return 0;
}