#include <cstdio>

typedef unsigned long ulong;

struct CActionUseDef {};

ulong __fastcall CActionUseDef_GetSizeofClass(const CActionUseDef* self)
{
    (void)self;
    return 0x34;
}

int main()
{
    CActionUseDef d;
    ulong r = CActionUseDef_GetSizeofClass(&d);
    if (r == 0x34) {
        std::printf("CActionUseDef_0044c269_TEST PASS\n");
        return 0;
    }
    std::printf("CActionUseDef_0044c269_TEST FAIL got %lu\n", r);
    return 1;
}