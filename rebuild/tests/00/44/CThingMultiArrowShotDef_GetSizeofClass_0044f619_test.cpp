#include <cstdio>

struct CThingMultiArrowShotDef { int dummy; };

unsigned long __fastcall CThingMultiArrowShotDef_GetSizeofClass(const CThingMultiArrowShotDef* self)
{
    (void)self;
    return 0x54;
}

int main()
{
    CThingMultiArrowShotDef obj;
    unsigned long r = CThingMultiArrowShotDef_GetSizeofClass(&obj);
    if (r != 0x54) {
        std::printf("CThingMultiArrowShotDef_0044f619_TEST FAIL got %lu\n", r);
        return 1;
    }
    std::printf("CThingMultiArrowShotDef_0044f619_TEST PASS\n");
    return 0;
}