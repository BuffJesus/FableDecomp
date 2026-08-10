
#include <cstdio>

struct QCard {
    char pad0[0xc];
    unsigned char* f0c;
    int pad10;
    int f14;
};

bool __fastcall IsCore_2(QCard* self)
{
    if (self->f14 <= -1)
    {
        if (self->f0c[0x5a] != 0)
            return true;
    }
    return false;
}

int main()
{
    unsigned char buf[0x60];
    for (int i = 0; i < 0x60; ++i) buf[i] = 0;
    QCard c; c.f0c = buf;
    c.f14 = 0; buf[0x5a] = 1;
    if (IsCore_2(&c) != false) { printf("FAIL1\n"); return 1; }
    c.f14 = -1; buf[0x5a] = 1;
    if (IsCore_2(&c) != true) { printf("FAIL2\n"); return 1; }
    c.f14 = -5; buf[0x5a] = 7;
    if (IsCore_2(&c) != true) { printf("FAIL3\n"); return 1; }
    c.f14 = -1; buf[0x5a] = 0;
    if (IsCore_2(&c) != false) { printf("FAIL4\n"); return 1; }
    printf("OK_007022a0\n");
    return 0;
}