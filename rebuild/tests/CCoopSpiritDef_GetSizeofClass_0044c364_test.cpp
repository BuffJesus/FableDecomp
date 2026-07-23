#include <cstdio>

struct CCoopSpiritDef { int dummy; };

unsigned long __fastcall CCoopSpiritDef_GetSizeofClass(const CCoopSpiritDef* self)
{
    (void)self;
    return 0x3c;
}

int main()
{
    CCoopSpiritDef d;
    unsigned long r = CCoopSpiritDef_GetSizeofClass(&d);
    if (r == 0x3c) {
        std::printf("CCoopSpiritDef_0044c364_TEST PASS\n");
        return 0;
    }
    std::printf("CCoopSpiritDef_0044c364_TEST FAIL got=%lu\n", r);
    return 1;
}