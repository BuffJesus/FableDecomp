#include <stdio.h>

struct CGuiVarTransferBool {
    void* target;
    void* context;
    char  pad8[4];
    bool  value;
    unsigned char done;
};

static void* g_a;
static void* g_b;
static bool  g_c;
static int   g_called;

extern "C" void __fastcall Helper_4d2470(void* a, void* b, bool c)
{
    g_a = a; g_b = b; g_c = c; g_called = 1;
}

void __fastcall TransferToGui(CGuiVarTransferBool* self);

int main()
{
    CGuiVarTransferBool o;
    o.target = (void*)0x1111;
    o.context = (void*)0x2222;
    o.value = true;
    o.done = 0x77;

    TransferToGui(&o);

    if (g_called && g_a == (void*)0x1111 && g_b == (void*)0x2222
        && g_c == true && o.done == 0)
        printf("004d2600_TEST PASS\n");
    else
        printf("FAIL a=%p b=%p c=%d done=%d\n", g_a, g_b, (int)g_c, o.done);
    return 0;
}