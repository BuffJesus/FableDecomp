#include <cstdio>

struct CSpecialAbilitiesBulletTimeDef { char pad[0x18c]; };

unsigned long __fastcall CSpecialAbilitiesBulletTimeDef_GetSizeofClass(const CSpecialAbilitiesBulletTimeDef* self)
{
    (void)self;
    return 0x18c;
}

int main()
{
    CSpecialAbilitiesBulletTimeDef obj;
    unsigned long r = CSpecialAbilitiesBulletTimeDef_GetSizeofClass(&obj);
    if (r == 0x18c) {
        std::printf("CSpecialAbilitiesBulletTimeDef_004575da_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL: got %lu\n", r);
    return 1;
}