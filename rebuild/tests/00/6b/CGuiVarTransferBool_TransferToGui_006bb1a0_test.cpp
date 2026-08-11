#include <cstdio>

struct GuiTarget { int probe; };

struct CGuiVarTransferBool {
    GuiTarget* target;
    int        arg2;
    int        pad8;
    bool       value;
    char       flag;
};

static GuiTarget* g_self;
static int        g_edx;
static bool       g_val;
static int        g_calls;

void __fastcall GuiSetBool(GuiTarget* self, int edx, bool val)
{
    g_self = self; g_edx = edx; g_val = val; ++g_calls;
}

void __fastcall TransferToGui(CGuiVarTransferBool* self)
{
    GuiSetBool(self->target, self->arg2, self->value);
    self->flag = 0;
}

int main()
{
    GuiTarget t; t.probe = 7;
    CGuiVarTransferBool o;
    o.target = &t; o.arg2 = 0x1234; o.pad8 = 0; o.value = 1; o.flag = 0x55;
    TransferToGui(&o);
    bool ok = (g_calls==1) && (g_self==&t) && (g_edx==0x1234) && (g_val==true) && (o.flag==0);

    o.value = 0; o.flag = 0x55; g_calls=0;
    TransferToGui(&o);
    ok = ok && (g_calls==1) && (g_val==false) && (o.flag==0);

    if (ok) printf("006bb1a0_TEST PASS\n");
    else    printf("FAIL calls=%d val=%d flag=%d\n", g_calls, (int)g_val, (int)o.flag);
    return 0;
}