#include <cstdio>

typedef unsigned long ulong;

ulong __fastcall CBonusItemDef_GetSizeofClass(const void* self)
{
    (void)self;
    return 0x74;
}

int main()
{
    int dummy = 0;
    ulong r = CBonusItemDef_GetSizeofClass(&dummy);
    if (r == 0x74) {
        std::printf("CBonusItemDef_0044c390_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}