#include <cstdio>

struct CThingHolySiteDef;

unsigned long __fastcall CThingHolySiteDef_GetSizeofClass(const CThingHolySiteDef* self)
{
    return 0x80;
}

int main()
{
    unsigned long r = CThingHolySiteDef_GetSizeofClass(0);
    if (r == 0x80) {
        std::printf("CThingHolySiteDef_00454dc5_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}