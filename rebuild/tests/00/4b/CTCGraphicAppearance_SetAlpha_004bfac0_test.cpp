#include <stdio.h>

struct Sub {
    int touched;
    void Touch();
};
static Sub g_sub;
void Sub::Touch() { g_sub.touched++; }

struct CTCGraphicAppearance {
    char pad0[4];        // +0x00
    Sub* f4;             // +0x04
    char pad8[0x58 - 8]; // +0x08 .. +0x57
    unsigned char flags; // +0x58
    char pad59;          // +0x59
    unsigned char alpha; // +0x5a
    void SetAlpha(unsigned char a);
};

void CTCGraphicAppearance::SetAlpha(unsigned char a)
{
    this->alpha = a;
    this->f4->Touch();
    this->flags |= 4;
}

int main()
{
    CTCGraphicAppearance obj;
    obj.flags = 0x01;
    obj.alpha = 0x00;
    obj.f4 = &g_sub;
    g_sub.touched = 0;

    obj.SetAlpha(0xAB);

    bool ok = (obj.alpha == 0xAB)
           && (obj.flags == 0x05)      // 0x01 | 0x04
           && (g_sub.touched == 1);
    if (ok) printf("SETALPHA_OK\n");
    else    printf("SETALPHA_FAIL a=%u f=%u t=%d\n", obj.alpha, obj.flags, g_sub.touched);
    return ok ? 0 : 1;
}