#include <cstdio>

struct CInventoryCategoryDef { char pad[0x58]; };

unsigned long __fastcall CInventoryCategoryDef_GetSizeofClass(CInventoryCategoryDef* self)
{
    (void)self;
    return 0x58;
}

int main()
{
    CInventoryCategoryDef obj;
    unsigned long r = CInventoryCategoryDef_GetSizeofClass(&obj);
    if (r == 0x58) {
        std::printf("CInventoryCategoryDef_00455584_TEST PASS\n");
        return 0;
    }
    std::printf("CInventoryCategoryDef_00455584_TEST FAIL got=%lu\n", r);
    return 1;
}