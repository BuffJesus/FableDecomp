#include <cstdio>

typedef unsigned long ulong;

ulong __fastcall CSpecialAbilitiesForcePushDataDef_GetSizeofClass(const void* self)
{
    (void)self;
    return 0x2c;
}

int main()
{
    int dummy = 0;
    ulong r = CSpecialAbilitiesForcePushDataDef_GetSizeofClass(&dummy);
    if (r == 0x2c) {
        std::printf("CSpecialAbilitiesForcePushDataDef_0044c329_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}