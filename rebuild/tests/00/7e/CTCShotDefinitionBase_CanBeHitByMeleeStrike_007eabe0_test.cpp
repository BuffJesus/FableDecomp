#include <cstdio>

struct CTCShotDefinitionBase {
    char pad0[0xC];
    bool* pShot; // +0x0C
};

bool __fastcall CanBeHitByMeleeStrike(CTCShotDefinitionBase* self)
{
    return self->pShot[0x2C];
}

int main()
{
    bool buf[0x40];
    for (int i = 0; i < 0x40; ++i) buf[i] = false;
    CTCShotDefinitionBase d;
    d.pShot = buf;

    buf[0x2C] = false;
    if (CanBeHitByMeleeStrike(&d) != false) { printf("FAIL zero\n"); return 1; }
    buf[0x2C] = true;
    if (CanBeHitByMeleeStrike(&d) != true) { printf("FAIL one\n"); return 1; }

    printf("OK_007eabe0\n");
    return 0;
}