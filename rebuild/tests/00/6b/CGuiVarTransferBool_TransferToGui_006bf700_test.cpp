#include <stdio.h>

struct CGuiVarTransferBool
{
    void*         field0;
    void*         field4;
    int           field8;
    bool          valueC;
    unsigned char valueD;
};

static void*         g_lastSelf;
static void*         g_lastEdx;
static int           g_lastValue;
static int           g_called;

extern void __fastcall guivar_bool_helper(void* self, void* edx, bool value)
{
    g_lastSelf  = self;
    g_lastEdx   = edx;
    g_lastValue = value ? 1 : 0;
    g_called++;
}

void __fastcall TransferToGui(CGuiVarTransferBool* self);

int main()
{
    int a, b;
    CGuiVarTransferBool o;
    o.field0 = &a;
    o.field4 = &b;
    o.field8 = 0;
    o.valueC = 1;
    o.valueD = 0xEE;

    TransferToGui(&o);

    if (g_called == 1 &&
        g_lastSelf == &a &&
        g_lastEdx == &b &&
        g_lastValue == 1 &&
        o.valueD == 0)
    {
        printf("006bf700_TEST PASS\n");
    }
    else
    {
        printf("FAIL called=%d value=%d valueD=%d\n", g_called, g_lastValue, (int)o.valueD);
    }
    return 0;
}