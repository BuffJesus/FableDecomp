#include <stdio.h>

struct CGuiVarTransferBool {
    void* f0;
    void* f4;
    char  pad8[4];
    bool  f0c;
    bool  f0d;
};

static void* g_ecx;
static void* g_edx;
static int   g_b;
static int   g_called;

void __fastcall helper_4bbb40(void* ecx, void* edx, bool b)
{
    g_ecx = ecx; g_edx = edx; g_b = b ? 0xAB : 0; g_called = 1;
}

void __fastcall TransferToGui(CGuiVarTransferBool* self);

int main()
{
    CGuiVarTransferBool o;
    o.f0 = (void*)0x11110000;
    o.f4 = (void*)0x22220000;
    o.f0c = true;
    o.f0d = true;
    g_called = 0;

    TransferToGui(&o);

    if (g_called == 1 &&
        g_ecx == (void*)0x11110000 &&
        g_edx == (void*)0x22220000 &&
        g_b == 0xAB &&
        o.f0d == 0)
    {
        printf("004bbbc0_TEST PASS\n");
    } else {
        printf("FAIL called=%d ecx=%p edx=%p b=%x f0d=%d\n",
               g_called, g_ecx, g_edx, g_b, (int)o.f0d);
    }
    return 0;
}