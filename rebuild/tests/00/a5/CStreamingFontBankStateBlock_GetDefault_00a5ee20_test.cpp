#include <cstdio>

struct CBankStateBlock {
    int f0;
    int f4;
    int f8;
    int fC;
    char f10;
};

struct CStreamingFontBankStateBlock {
    void GetDefault(CBankStateBlock &out);
};

void CStreamingFontBankStateBlock::GetDefault(CBankStateBlock &out)
{
    out.f4 = 0;
    out.f8 = 0x190;
    out.fC = 0;
    out.f10 = 0;
}

int main()
{
    CBankStateBlock b;
    b.f0 = 0x11111111;
    b.f4 = 0x22222222;
    b.f8 = 0x33333333;
    b.fC = 0x44444444;
    b.f10 = 0x55;
    CStreamingFontBankStateBlock s;
    s.GetDefault(b);
    if (b.f0 == 0x11111111 && b.f4 == 0 && b.f8 == 0x190 && b.fC == 0 && b.f10 == 0) {
        printf("GETDEFAULT_OK\n");
        return 0;
    }
    printf("GETDEFAULT_FAIL\n");
    return 1;
}