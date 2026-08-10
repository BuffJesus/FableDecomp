#include <cstdio>

struct CFontBankStateBlock {
    int   f0;
    int   f4;
    int   f8;
    char  fc;
};

void __stdcall GetDefault(CFontBankStateBlock* p)
{
    p->f4 = 0;
    p->f8 = 0x190;
    p->fc = 0;
}

int main()
{
    CFontBankStateBlock b;
    b.f0 = 0x11111111;
    b.f4 = 0x22222222;
    b.f8 = 0x33333333;
    b.fc = (char)0x44;
    GetDefault(&b);
    if (b.f0 != 0x11111111) { printf("FAIL f0\n"); return 1; }
    if (b.f4 != 0) { printf("FAIL f4\n"); return 1; }
    if (b.f8 != 0x190) { printf("FAIL f8\n"); return 1; }
    if (b.fc != 0) { printf("FAIL fc\n"); return 1; }
    printf("OK_00a632d0\n");
    return 0;
}