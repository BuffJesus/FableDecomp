#include <stdio.h>

struct CGuiVarTransferBool {
    void*        field_0;
    int          field_4;
    char         pad_8[4];
    unsigned char value;
    unsigned char flag;
};

static void*         g_ecx;
static int           g_edx;
static unsigned char g_val;
static int           g_called;

void __fastcall CGuiVarTransferBool_Sink(void* ecx, int edx, unsigned char val)
{
    g_ecx = ecx;
    g_edx = edx;
    g_val = val;
    g_called = 1;
}

void __fastcall CGuiVarTransferBool_TransferToGui(CGuiVarTransferBool* thisptr);

int main()
{
    CGuiVarTransferBool o;
    o.field_0 = (void*)0x11223344;
    o.field_4 = 0x55667788;
    o.value   = 0xAB;
    o.flag    = 0x99;

    CGuiVarTransferBool_TransferToGui(&o);

    if (g_called == 1 &&
        g_ecx == (void*)0x11223344 &&
        g_edx == 0x55667788 &&
        g_val == 0xAB &&
        o.flag == 0)
    {
        printf("0076c180_TEST PASS\n");
    }
    else
    {
        printf("FAIL called=%d ecx=%p edx=%x val=%x flag=%x\n",
               g_called, g_ecx, g_edx, (int)g_val, (int)o.flag);
    }
    return 0;
}