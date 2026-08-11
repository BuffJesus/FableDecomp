#include <cstdio>

struct CGuiVarTarget;

struct CGuiVarTransferBool {
    CGuiVarTarget* target;
    int            field;
    int            pad8;
    bool           value;
    unsigned char  dirty;
};

static CGuiVarTarget* g_target;
static int g_field;
static int g_value;
static int g_called;

void __fastcall CGuiVarTransferBool_helper(CGuiVarTarget* target, int field, bool value)
{
    g_target = target;
    g_field  = field;
    g_value  = value;
    g_called = 1;
}

void __fastcall CGuiVarTransferBool_TransferToGui(CGuiVarTransferBool* self)
{
    CGuiVarTransferBool_helper(self->target, self->field, self->value);
    self->dirty = 0;
}

int main()
{
    CGuiVarTransferBool o;
    o.target = (CGuiVarTarget*)0x12345678;
    o.field  = 99;
    o.pad8   = 0;
    o.value  = 1;
    o.dirty  = 0xAB;

    CGuiVarTransferBool_TransferToGui(&o);

    bool ok = g_called == 1
           && g_target == (CGuiVarTarget*)0x12345678
           && g_field  == 99
           && g_value  == 1
           && o.dirty  == 0;
    if (ok) printf("007442d0_TEST PASS\n");
    else    printf("FAIL c=%d t=%p f=%d v=%d d=%d\n", g_called, (void*)g_target, g_field, g_value, (int)o.dirty);
    return 0;
}