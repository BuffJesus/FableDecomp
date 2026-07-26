#include <cstdio>
struct CThingVillageDef {};
unsigned long __fastcall CThingVillageDef_GetSizeofClass(const CThingVillageDef* self)
{
    (void)self;
    return 0x80;
}
int main()
{
    CThingVillageDef d;
    unsigned long r = CThingVillageDef_GetSizeofClass(&d);
    if (r == 0x80) {
        std::printf("CThingVillageDef_00454f59_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}