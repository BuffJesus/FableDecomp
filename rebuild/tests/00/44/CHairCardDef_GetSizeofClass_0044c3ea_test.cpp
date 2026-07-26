#include <cstdio>

typedef unsigned long ulong;

ulong __fastcall CHairCardDef_GetSizeofClass(const void* self)
{
    (void)self;
    return 0x2c;
}

int main()
{
    ulong r = CHairCardDef_GetSizeofClass(0);
    if (r == 0x2c) {
        std::printf("CHairCardDef_0044c3ea_TEST PASS\n");
        return 0;
    }
    std::printf("CHairCardDef_0044c3ea_TEST FAIL got %lu\n", r);
    return 1;
}