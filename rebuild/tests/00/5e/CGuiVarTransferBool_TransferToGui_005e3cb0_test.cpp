#include <stdio.h>

struct CGuiVarTransferBool {
    void* f0;
    void* f4;
    char  pad8[4];
    unsigned char f0c;
    unsigned char f0d;
};

extern "C" void __fastcall gui_bool_helper(void* ecx, void* edx, unsigned char v);

void __fastcall TransferToGui(CGuiVarTransferBool* self);

static void* g_ecx;
static void* g_edx;
static unsigned char g_v;
static int g_called;

extern "C" void __fastcall gui_bool_helper(void* ecx, void* edx, unsigned char v)
{
    g_ecx = ecx; g_edx = edx; g_v = v; g_called = 1;
}

int main()
{
    CGuiVarTransferBool o;
    int a = 111, b = 222;
    o.f0 = &a;
    o.f4 = &b;
    o.f0c = 0x5A;
    o.f0d = 0xFF;

    TransferToGui(&o);

    if (g_called == 1 && g_ecx == &a && g_edx == &b && g_v == 0x5A && o.f0d == 0)
        printf("005e3cb0_TEST PASS\n");
    else
        printf("FAIL called=%d v=%d f0d=%d\n", g_called, g_v, o.f0d);
    return 0;
}