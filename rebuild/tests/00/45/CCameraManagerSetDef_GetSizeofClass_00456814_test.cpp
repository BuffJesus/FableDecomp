#include <cstdio>

typedef unsigned long ulong;

ulong __fastcall CCameraManagerSetDef_GetSizeofClass(const void* self)
{
    (void)self;
    return 0x4c;
}

int main()
{
    char dummy = 0;
    ulong r = CCameraManagerSetDef_GetSizeofClass(&dummy);
    if (r != 0x4c) { std::printf("FAIL got %lu\n", r); return 1; }
    std::printf("CCameraManagerSetDef_00456814_TEST PASS\n");
    return 0;
}