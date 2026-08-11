#include <cstdio>

struct CGuiVarTransferBool {
    void*         gui;
    int           edxv;
    char          pad8, pad9, pada, padb;
    unsigned char value;
    unsigned char dirty;
};

static void*  g_self;
static int    g_edx;
static int    g_arg;
static int    g_called;

void __fastcall Helper_7597C0(void* self, int edx, unsigned char arg)
{
    g_self = self; g_edx = edx; g_arg = arg; g_called = 1;
}

void __fastcall TransferToGui(CGuiVarTransferBool* self)
{
    Helper_7597C0(self->gui, self->edxv, self->value);
    self->dirty = 0;
}

int main()
{
    CGuiVarTransferBool o;
    o.gui  = (void*)0x1234;
    o.edxv = 0x5678;
    o.value = 0xAB;
    o.dirty = 0xFF;

    TransferToGui(&o);

    if (g_called && g_self == (void*)0x1234 && g_edx == 0x5678
        && g_arg == 0xAB && o.dirty == 0)
        printf("00759820_TEST PASS\n");
    else
        printf("FAIL self=%p edx=%x arg=%x dirty=%x\n", g_self, g_edx, g_arg, o.dirty);
    return 0;
}