#include <cstdio>

struct CGuiVarTransferBool {
    void* f0;
    void* f4;
    unsigned char f8, f9, fa, fb;
    bool f0c;
    bool f0d;
};

static void* g_ecx;
static void* g_edx;
static int   g_val;
static int   g_called;

void __fastcall CGuiVarTransferBool_helper(void* ecx, void* edx, bool val)
{
    g_ecx = ecx; g_edx = edx; g_val = val ? 1 : 0; g_called = 1;
}

void __fastcall CGuiVarTransferBool_TransferToGui(CGuiVarTransferBool* self);

int main()
{
    CGuiVarTransferBool o;
    o.f0 = (void*)0x11112222;
    o.f4 = (void*)0x33334444;
    o.f0c = true;
    o.f0d = true;
    g_called = 0;

    CGuiVarTransferBool_TransferToGui(&o);

    if (g_called && g_ecx == (void*)0x11112222 && g_edx == (void*)0x33334444
        && g_val == 1 && o.f0d == false)
        printf("006a3f80_TEST PASS\n");
    else
        printf("FAIL called=%d val=%d f0d=%d\n", g_called, g_val, o.f0d);
    return 0;
}