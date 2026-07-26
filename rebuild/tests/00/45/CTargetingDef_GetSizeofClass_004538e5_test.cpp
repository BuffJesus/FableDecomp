#include <cstdio>

typedef unsigned long ulong;

ulong __fastcall CTargetingDef_GetSizeofClass(const void* self)
{
    (void)self;
    return 0x90;
}

int main()
{
    ulong r = CTargetingDef_GetSizeofClass(0);
    if (r == 0x90) {
        std::printf("CTargetingDef_004538e5_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}