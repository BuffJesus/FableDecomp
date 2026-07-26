#include <cstdio>

struct CThingBuildingDef {};

unsigned long __fastcall CThingBuildingDef_GetSizeofClass(const CThingBuildingDef* self)
{
    (void)self;
    return 0xa4;
}

int main()
{
    CThingBuildingDef obj;
    unsigned long r = CThingBuildingDef_GetSizeofClass(&obj);
    if (r == 0xa4) {
        std::printf("CThingBuildingDef_00454b88_TEST PASS\n");
        return 0;
    }
    std::printf("CThingBuildingDef_00454b88_TEST FAIL got %lu\n", r);
    return 1;
}