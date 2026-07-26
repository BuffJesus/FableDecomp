#include <cstdio>

typedef unsigned long ulong;

struct CMaterialDef;

ulong __fastcall CMaterialDef_GetSizeofClass(const CMaterialDef* self)
{
    (void)self;
    return 0x98;
}

int main()
{
    ulong r = CMaterialDef_GetSizeofClass(0);
    if (r != 0x98) {
        std::printf("CMaterialDef_00455d22_TEST FAIL got=%lu\n", r);
        return 1;
    }
    std::printf("CMaterialDef_00455d22_TEST PASS\n");
    return 0;
}