#include <stdio.h>

struct CGuiVarTransferBool
{
    void*         field_0;
    int           field_4;
    int           pad_8;
    unsigned char field_C;
    unsigned char field_D;

    void TransferToGui();
};

static void*         g_a;
static int           g_b;
static unsigned char g_c;
static int           g_calls;

void __fastcall CGuiVarTransferBool_Emit(void* a, int b, unsigned char c)
{
    g_a = a;
    g_b = b;
    g_c = c;
    g_calls++;
}

int main()
{
    CGuiVarTransferBool o;
    int dummy = 0;
    o.field_0 = &dummy;
    o.field_4 = 0x12345678;
    o.pad_8   = 0;
    o.field_C = 0xAB;
    o.field_D = 0x77;

    o.TransferToGui();

    if (g_calls == 1 && g_a == &dummy && g_b == 0x12345678 &&
        g_c == 0xAB && o.field_D == 0)
    {
        printf("TRANSFERBOOL_OK\n");
        return 0;
    }
    printf("FAIL calls=%d b=%x c=%x d=%x\n", g_calls, g_b, g_c, o.field_D);
    return 1;
}