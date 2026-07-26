#include <cstdio>

struct CThingMarkerDef {};

unsigned long __fastcall CThingMarkerDef_GetSizeofClass(const CThingMarkerDef* self)
{
    (void)self;
    return 0x88;
}

int main()
{
    CThingMarkerDef d;
    unsigned long r = CThingMarkerDef_GetSizeofClass(&d);
    if (r == 0x88) {
        std::printf("CThingMarkerDef_00454e1b_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}