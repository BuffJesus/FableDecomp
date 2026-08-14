#include <stdio.h>

struct CTCHeroStats {
    char pad[0x19c];
    float f19c;
    float f1a0;
    char pad2[0x1ac - 0x1a4];
    int   i1ac;
    char pad3[0x1b8 - 0x1b0];
    unsigned char b1b8;
};

void __fastcall SoberUp(CTCHeroStats* self)
{
    self->i1ac = -1;
    self->f19c = 0.0f;
    self->b1b8 = 1;
    self->f1a0 = 0.0f;
}

int main()
{
    CTCHeroStats s;
    s.i1ac = 12345;
    s.f19c = 9.0f;
    s.f1a0 = 9.0f;
    s.b1b8 = 0;
    SoberUp(&s);
    if (s.i1ac == -1 && s.f19c == 0.0f && s.f1a0 == 0.0f && s.b1b8 == 1) {
        printf("SOBERUP_OK\n");
    } else {
        printf("SOBERUP_FAIL\n");
    }
    return 0;
}