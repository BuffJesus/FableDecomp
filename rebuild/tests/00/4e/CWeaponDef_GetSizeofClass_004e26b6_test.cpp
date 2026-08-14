#include <cstdio>

struct CWeaponDef;

unsigned long __fastcall GetSizeofClass(CWeaponDef *self)
{
    (void)self;
    return 0xe4;
}

int main()
{
    unsigned long r = GetSizeofClass((CWeaponDef*)0);
    if (r == 0xe4u) {
        printf("BEHAVIOR_OK\n");
    } else {
        printf("BEHAVIOR_FAIL %lu\n", r);
    }
    return 0;
}