#include <cstdio>

struct CThingNoiseDef {};

unsigned long __fastcall CThingNoiseDef_GetSizeofClass(const CThingNoiseDef* self)
{
    (void)self;
    return 0xa4;
}

int main()
{
    CThingNoiseDef obj;
    unsigned long r = CThingNoiseDef_GetSizeofClass(&obj);
    if (r == 0xa4) {
        std::printf("CThingNoiseDef_00455f14_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}