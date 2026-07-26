#include <cstdio>

struct CThingPhysicalSwitchDef {};

unsigned long __fastcall CThingPhysicalSwitchDef_GetSizeofClass(const CThingPhysicalSwitchDef* self)
{
    (void)self;
    return 0x94;
}

int main()
{
    CThingPhysicalSwitchDef obj;
    unsigned long r = CThingPhysicalSwitchDef_GetSizeofClass(&obj);
    if (r == 0x94) {
        std::printf("CThingPhysicalSwitchDef_00454fbd_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}