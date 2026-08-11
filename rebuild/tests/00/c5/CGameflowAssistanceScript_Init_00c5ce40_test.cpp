#include <stdio.h>

struct CGameflowAssistanceScript {
    int f0;
    int f4;
    int f8;
    int fc;
    int f10;
    int f14;
    int f18;
    int f1c;
};

void __fastcall CGameflowAssistanceScript_Init(CGameflowAssistanceScript *self);

int main()
{
    CGameflowAssistanceScript o;
    o.f0 = 0x11111111;
    o.f4 = 1; o.f8 = 1; o.fc = 1; o.f10 = 1; o.f14 = 1; o.f18 = 1; o.f1c = 1;
    CGameflowAssistanceScript_Init(&o);
    if (o.f0 == 0x11111111 && o.f4==0 && o.f8==0 && o.fc==0 && o.f10==0 && o.f14==0 && o.f18==0 && o.f1c==0)
        printf("00c5ce40_TEST PASS\n");
    else
        printf("FAIL\n");
    return 0;
}